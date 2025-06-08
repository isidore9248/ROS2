#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>

#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <nav_msgs/msg/path.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
// #include <tf2_geometry_msgs/tf2_geometry_msgs.hpp> // For converting tf2::Quaternion to geometry_msgs::msg::Quaternion

// 定义重力加速度常数 (m/s^2)
const double G_ACCEL = 9.80665;
// 将角度转换为弧度
const double DEG_TO_RAD = M_PI / 180.0;

// 数据结构体
struct IMUData
{
    double time;
    double ax, ay, az;     // 加速度 (g)
    double wx, wy, wz;     // 角速度 (deg/s)
    double q0, q1, q2, q3; // 四元数
    double lon, lat;       // 经纬度 (deg)
    double gps_v;          // GPS 速度 (km/h)
};

// 四元数结构体
struct Quaternion
{
    double w, x, y, z;

    // 构造函数
    Quaternion(double w_ = 1.0, double x_ = 0.0, double y_ = 0.0, double z_ = 0.0)
        : w(w_), x(x_), y(y_), z(z_) {}

    // 四元数乘法
    Quaternion operator*(const Quaternion &other) const
    {
        return Quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w);
    }

    // 四元数共轭
    Quaternion conjugate() const
    {
        return Quaternion(w, -x, -y, -z);
    }

    // 四元数归一化
    void normalize()
    {
        double mag = std::sqrt(w * w + x * x + y * y + z * z);
        if (mag > 0)
        {
            w /= mag;
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }
};

// 向量结构体 (用于加速度和速度)
struct Vector3
{
    double x, y, z;

    Vector3(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0)
        : x(x_), y(y_), z(z_) {}

    // 向量加法
    Vector3 operator+(const Vector3 &other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // 向量减法
    Vector3 operator-(const Vector3 &other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // 向量与标量乘法
    Vector3 operator*(double scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // 向量与标量除法
    Vector3 operator/(double scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
};

// 将欧拉角 (roll, pitch, yaw) 转换为四元数
Quaternion toQuaternion(double roll_deg, double pitch_deg, double yaw_deg)
{
    double roll = roll_deg * DEG_TO_RAD;
    double pitch = pitch_deg * DEG_TO_RAD;
    double yaw = yaw_deg * DEG_TO_RAD;

    double cy = std::cos(yaw * 0.5);
    double sy = std::sin(yaw * 0.5);
    double cp = std::cos(pitch * 0.5);
    double sp = std::sin(pitch * 0.5);
    double cr = std::cos(roll * 0.5);
    double sr = std::sin(roll * 0.5);

    return Quaternion(
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy);
}

// 将四元数旋转向量 (从传感器坐标系到导航坐标系)
Vector3 rotateVectorByQuaternion(const Vector3 &vec, const Quaternion &q)
{
    Quaternion p(0.0, vec.x, vec.y, vec.z);
    Quaternion q_conj = q.conjugate();
    Quaternion rotated_p = q * p * q_conj;
    return Vector3(rotated_p.x, rotated_p.y, rotated_p.z);
}

// 解析时间字符串为秒数 (从00:00:00.000开始)
double parseTime(const std::string &time_str)
{
    int h = std::stoi(time_str.substr(0, 2));
    int m = std::stoi(time_str.substr(3, 2));
    int s = std::stoi(time_str.substr(6, 2));
    int ms = std::stoi(time_str.substr(9, 3));
    return h * 3600.0 + m * 60.0 + s + ms / 1000.0;
}

class TrajectoryVisualization : public rclcpp::Node
{
public:
    TrajectoryVisualization() : Node("trajectory_visualization")
    {
        pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("trajectory_pose", 10);
        twist_publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("trajectory_twist", 10);
        marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("trajectory_marker", 10);
        path_pub_ = this->create_publisher<nav_msgs::msg::Path>("trajectory_path", 10);

        readDataFile();

        if (!imu_data_vec_.empty())
        {
            // 初始化状态变量
            current_q_ = Quaternion(imu_data_vec_[0].q0, imu_data_vec_[0].q1, imu_data_vec_[0].q2, imu_data_vec_[0].q3);
            current_q_.normalize();
            prev_time_ = imu_data_vec_[0].time;

            // 设置定时器，以数据采样频率发布数据
            // 假设数据采样频率是100ms (10Hz)，根据data3.txt的时间间隔来确定
            // 实际应用中，应该根据IMU的实际输出频率来设置
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(100), // 假设10Hz
                std::bind(&TrajectoryVisualization::publishImuData, this));
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "No valid IMU data loaded from file.");
        }
    }

private:
    std::vector<IMUData> imu_data_vec_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr twist_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::vector<geometry_msgs::msg::PoseStamped> trajectory_poses_;

    // 状态变量
    Quaternion current_q_;
    Vector3 velocity_ = Vector3(0.0, 0.0, 0.0);
    Vector3 position_ = Vector3(0.0, 0.0, 0.0);
    Vector3 initial_position_ = Vector3(0.0, 0.0, 0.0); // 添加初始位置
    bool is_first_point_ = true;                        // 用于标记第一个数据点
    double prev_time_;
    size_t current_data_index_ = 0;

    // 简单的互补滤波器参数 (可以根据实际情况调整)
    const double ACCEL_WEIGHT = 0.01; // 加速度计对姿态校正的权重

    void readDataFile()
    {
        std::ifstream infile("/home/ros2/ROS2Folder/Event/data.txt");
        if (!infile.is_open())
        {
            RCLCPP_ERROR(this->get_logger(), "错误: 无法打开 data3.txt 文件");
            return;
        }

        std::string line;
        // 跳过文件头
        std::getline(infile, line); // StartTime: ...
        std::getline(infile, line); // address Time(s) ax(g) ...

        // 读取数据
        while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            std::string address_str, time_str;
            IMUData imu_data;

            iss >> address_str >> time_str;
            imu_data.time = parseTime(time_str);

            // 读取加速度、角速度、欧拉角、温度、磁力计、压力、高度、经纬度、GPS速度、四元数、SV、PDOP、HDOP、VDOP
            // 确保读取顺序与文件头一致
            // ax(g) ay(g) az(g) wx(deg/s) wy(deg/s) wz(deg/s) AngleX(deg) AngleY(deg) AngleZ(deg) T(°) hx hy hz Pressure(Pa) Altitude(m) Lon(deg) Lat(deg) GPSHeight(m) GPSYaw(deg) GPSV(km/h) q0 q1 q2 q3 SV PDOP HDOP VDOP
            std::string dummy;                                                                                                       // 用于跳过不需要的字段
            iss >> imu_data.ax >> imu_data.ay >> imu_data.az >> imu_data.wx >> imu_data.wy >> imu_data.wz >> dummy >> dummy >> dummy // AngleX, AngleY, AngleZ (我们将自己解算姿态)
                >> dummy >> dummy >> dummy >> dummy                                                                                  // T, hx, hy, hz
                >> dummy >> dummy                                                                                                    // Pressure, Altitude
                >> imu_data.lon >> imu_data.lat                                                                                      // Lon, Lat
                >> dummy >> dummy >> imu_data.gps_v                                                                                  // GPSHeight, GPSYaw, GPSV
                >> imu_data.q0 >> imu_data.q1 >> imu_data.q2 >> imu_data.q3;                                                         // q0, q1, q2, q3

            imu_data_vec_.push_back(imu_data);
        }
        infile.close();
    }

    void publishImuData()
    {
        if (current_data_index_ >= imu_data_vec_.size())
        {
            RCLCPP_INFO(this->get_logger(), "所有数据已发布完毕。");
            timer_->cancel();
            return;
        }

        const IMUData &current_imu_data = imu_data_vec_[current_data_index_];
        double dt = current_imu_data.time - prev_time_;
        if (dt <= 0 && current_data_index_ > 0)
        { // 避免第一个数据点或时间戳重复
            prev_time_ = current_imu_data.time;
            current_data_index_++;
            return;
        }

        // 1. 姿态更新 (陀螺仪积分)
        // 将角速度从 deg/s 转换为 rad/s
        double wx_rad = current_imu_data.wx * DEG_TO_RAD;
        double wy_rad = current_imu_data.wy * DEG_TO_RAD;
        double wz_rad = current_imu_data.wz * DEG_TO_RAD;

        // 计算角增量四元数
        double angle_mag = std::sqrt(wx_rad * wx_rad + wy_rad * wy_rad + wz_rad * wz_rad) * dt;
        if (angle_mag > 0)
        {
            double sin_half_angle = std::sin(angle_mag / 2.0);
            double cos_half_angle = std::cos(angle_mag / 2.0);
            Quaternion delta_q(
                cos_half_angle,
                (wx_rad / angle_mag) * sin_half_angle,
                (wy_rad / angle_mag) * sin_half_angle,
                (wz_rad / angle_mag) * sin_half_angle);
            current_q_ = current_q_ * delta_q;
            current_q_.normalize();
        }

        // 2. 姿态校正 (加速度计) - 互补滤波思想
        // 将加速度从 g 转换为 m/s^2
        Vector3 accel_body(current_imu_data.ax * G_ACCEL, current_imu_data.ay * G_ACCEL, current_imu_data.az * G_ACCEL);

        // 期望的重力向量 (在导航坐标系下，z轴向下)
        Vector3 gravity_nav_expected(0.0, 0.0, -G_ACCEL);

        // 将当前姿态下的重力向量从导航坐标系转换到传感器坐标系
        // (这里需要四元数的共轭来从导航系转到体坐标系)
        Quaternion q_nav_to_body = current_q_.conjugate();
        Vector3 gravity_body_estimated = rotateVectorByQuaternion(gravity_nav_expected, q_nav_to_body);

        // 归一化加速度计测量值 (作为重力方向的测量)
        double accel_mag = std::sqrt(accel_body.x * accel_body.x + accel_body.y * accel_body.y + accel_body.z * accel_body.z);
        Vector3 accel_norm = accel_body / accel_mag;

        // 计算测量重力向量与估计重力向量之间的误差
        // 叉积得到旋转轴，点积得到旋转角度的余弦
        Vector3 error_axis(
            gravity_body_estimated.y * accel_norm.z - gravity_body_estimated.z * accel_norm.y,
            gravity_body_estimated.z * accel_norm.x - gravity_body_estimated.x * accel_norm.z,
            gravity_body_estimated.x * accel_norm.y - gravity_body_estimated.y * accel_norm.x);
        double error_mag = std::sqrt(error_axis.x * error_axis.x + error_axis.y * error_axis.y + error_axis.z * error_axis.z);

        if (error_mag > 0)
        {
            // 简单的比例校正
            double correction_angle = std::asin(error_mag) * ACCEL_WEIGHT;
            Quaternion correction_q(
                std::cos(correction_angle / 2.0),
                (error_axis.x / error_mag) * std::sin(correction_angle / 2.0),
                (error_axis.y / error_mag) * std::sin(correction_angle / 2.0),
                (error_axis.z / error_mag) * std::sin(correction_angle / 2.0));
            current_q_ = correction_q * current_q_; // 应用校正
            current_q_.normalize();
        }

        // 3. 加速度转换 (从传感器坐标系到导航坐标系)
        // 移除重力分量 (假设传感器坐标系z轴与重力方向一致，但实际需要旋转)
        // 这里我们假设加速度计测量的是包含重力的总加速度
        // 旋转后的加速度是导航坐标系下的加速度，需要减去重力
        Vector3 accel_nav = rotateVectorByQuaternion(accel_body, current_q_);
        accel_nav.z -= G_ACCEL; // 减去重力分量 (假设Z轴向上为正，重力向下为负)

        // 4. 速度更新 (积分加速度)
        Vector3 delta_v = accel_nav * dt;
        velocity_ = velocity_ + delta_v;

        // 5. 位置更新 (积分速度)
        Vector3 delta_pos = velocity_ * dt;
        position_ = position_ + delta_pos;

        // 如果是第一个点，记录初始位置
        if (is_first_point_)
        {
            initial_position_ = position_;
            is_first_point_ = false;
            RCLCPP_INFO(this->get_logger(), "记录初始位置: (%.3f, %.3f, %.3f)",
                        initial_position_.x, initial_position_.y, initial_position_.z);
        }

        // 计算相对于初始位置的位置
        Vector3 relative_position = Vector3(
            position_.x - initial_position_.x,
            position_.y - initial_position_.y,
            position_.z - initial_position_.z);

        // 发布姿态和位置信息
        geometry_msgs::msg::PoseStamped pose_msg;
        pose_msg.header.stamp = this->now();
        pose_msg.header.frame_id = "map";

        // 使用相对位置
        pose_msg.pose.position.x = relative_position.x;
        pose_msg.pose.position.y = relative_position.y;
        pose_msg.pose.position.z = relative_position.z;

        // 将自定义的Quaternion转换为geometry_msgs::msg::Quaternion
        pose_msg.pose.orientation.w = current_q_.w;
        pose_msg.pose.orientation.x = current_q_.x;
        pose_msg.pose.orientation.y = current_q_.y;
        pose_msg.pose.orientation.z = current_q_.z;

        pose_publisher_->publish(pose_msg);

        // 发布速度信息
        geometry_msgs::msg::TwistStamped twist_msg;
        twist_msg.header.stamp = this->now();
        twist_msg.header.frame_id = "map"; // 或者其他合适的坐标系

        twist_msg.twist.linear.x = velocity_.x;
        twist_msg.twist.linear.y = velocity_.y;
        twist_msg.twist.linear.z = velocity_.z;

        twist_publisher_->publish(twist_msg);

        RCLCPP_INFO(this->get_logger(), "时间: %.3f, 位置: (%.3f, %.3f, %.3f), 速度: (%.3f, %.3f, %.3f)",
                    current_imu_data.time, position_.x, position_.y, position_.z,
                    velocity_.x, velocity_.y, velocity_.z);

        // 添加当前姿态到轨迹中，使用相对位置
        geometry_msgs::msg::PoseStamped pose;
        pose.header.frame_id = "map";
        pose.header.stamp = this->now();
        pose.pose.position.x = relative_position.x;
        pose.pose.position.y = relative_position.y;
        pose.pose.position.z = relative_position.z;
        pose.pose.orientation.w = current_q_.w;
        pose.pose.orientation.x = current_q_.x;
        pose.pose.orientation.y = current_q_.y;
        pose.pose.orientation.z = current_q_.z;
        trajectory_poses_.push_back(pose);

        // 发布Path消息
        nav_msgs::msg::Path path;
        path.header.stamp = this->now();
        path.header.frame_id = "map";
        path.poses = trajectory_poses_;
        path_pub_->publish(path);

        // 发布轨迹Marker消息时也使用相对位置
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = "map";
        marker.header.stamp = this->now();
        marker.ns = "trajectory";
        marker.id = 0;
        marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.scale.x = 0.1;
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;

        geometry_msgs::msg::Point point;
        point.x = relative_position.x;
        point.y = relative_position.y;
        point.z = relative_position.z;
        marker.points.push_back(point);

        marker_pub_->publish(marker);

        // 发布高度Marker时也使用相对位置
        visualization_msgs::msg::Marker height_marker;
        height_marker.header.frame_id = "map";
        height_marker.header.stamp = this->now();
        height_marker.ns = "height";
        height_marker.id = 1;
        height_marker.type = visualization_msgs::msg::Marker::LINE_LIST;
        height_marker.action = visualization_msgs::msg::Marker::ADD;
        height_marker.scale.x = 0.05;
        height_marker.color.g = 1.0;
        height_marker.color.a = 0.5;

        geometry_msgs::msg::Point base, top;
        base.x = relative_position.x;
        base.y = relative_position.y;
        base.z = 0;

        top.x = relative_position.x;
        top.y = relative_position.y;
        top.z = relative_position.z;

        height_marker.points.push_back(base);
        height_marker.points.push_back(top);

        marker_pub_->publish(height_marker);

        prev_time_ = current_imu_data.time;
        current_data_index_++;
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TrajectoryVisualization>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
