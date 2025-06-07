#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <nav_msgs/msg/path.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

class TrajectoryVisualization : public rclcpp::Node
{
public:
    TrajectoryVisualization() : Node("trajectory_visualization")
    {
        // 创建publisher
        marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("trajectory_marker", 10);
        path_pub_ = this->create_publisher<nav_msgs::msg::Path>("trajectory_path", 10);

        // 读取数据并处理
        readDataFile();

        // 定时发布数据
        timer_ = this->create_wall_timer(100ms, std::bind(&TrajectoryVisualization::publishTrajectory, this));
    }

private:
    struct GPSData
    {
        double altitude;
        double longitude;
        double latitude;
        double x;
        double y;
        double z;
    };

    std::vector<GPSData> gps_data_;
    double origin_x_ = 0.0;
    double origin_y_ = 0.0;
    double origin_z_ = 0.0;

    void readDataFile()
    {
        std::ifstream file("/home/ros2/ROS2Folder/Event/data.txt");
        if (!file.is_open())
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to open data file");
            return;
        }
        std::string line;
        double x = 0.0, y = 0.0;   // 位置
        double vx = 0.0, vy = 0.0; // 速度
        double dt = 0.0;           // 时间间隔
        std::string prev_time;
        bool first_line = true;
        double ref_lon = 0.0, ref_lat = 0.0; // 参考经纬度
        bool first_point = true;             // 用于记录第一个点

        // 跳过标题行
        std::getline(file, line);
        std::getline(file, line);

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(iss, token, '\t'))
            {
                tokens.push_back(token);
            }

            if (tokens.size() >= 19)
            {
                std::string time_str = tokens[1];
                double ax = std::stod(tokens[2]); // ax(g)
                double ay = std::stod(tokens[3]); // ay(g)
                double wz = std::stod(tokens[8]); // wz(deg/s)

                // 获取GPS数据
                double lon = std::stod(tokens[17]); // 经度
                double lat = std::stod(tokens[18]); // 纬度
                double alt = std::stod(tokens[16]); // 高度(m)

                if (first_line)
                {
                    ref_lon = lon;
                    ref_lat = lat;
                }

                // 使用简单的经纬度转换为局部坐标系
                const double EARTH_RADIUS = 6378137.0; // 地球半径(m)
                double x_local = EARTH_RADIUS * (lon - ref_lon) * M_PI / 180.0 * std::cos(ref_lat * M_PI / 180.0);
                double y_local = EARTH_RADIUS * (lat - ref_lat) * M_PI / 180.0;

                // 转换加速度单位从g到m/s^2
                ax *= 9.81;
                ay *= 9.81;

                // 记录第一个点的位置作为原点
                if (first_point)
                {
                    origin_x_ = x_local;
                    origin_y_ = y_local;
                    origin_z_ = alt;
                    first_point = false;
                }

                // 相对于原点的坐标
                x_local -= origin_x_;
                y_local -= origin_y_;
                double z_local = alt - origin_z_;

                if (!first_line)
                {
                    // 计算时间间隔 (简单处理，仅获取毫秒差值)
                    dt = 0.1; // 假设采样率为10Hz

                    // 使用加速度积分得到速度
                    vx += ax * dt;
                    vy += ay * dt;

                    // 存储轨迹点
                    geometry_msgs::msg::PoseStamped pose;
                    pose.header.frame_id = "map";
                    pose.header.stamp = this->now();
                    pose.pose.position.x = x_local;
                    pose.pose.position.y = y_local;
                    pose.pose.position.z = z_local;

                    // 根据角速度设置方向
                    tf2::Quaternion q;
                    q.setRPY(0, 0, wz * M_PI / 180.0);
                    pose.pose.orientation.x = q.x();
                    pose.pose.orientation.y = q.y();
                    pose.pose.orientation.z = q.z();
                    pose.pose.orientation.w = q.w();

                    trajectory_poses_.push_back(pose);

                    // 存储当前点到gps_data_
                    GPSData gps_data;
                    gps_data.longitude = lon;
                    gps_data.latitude = lat;
                    gps_data.altitude = alt;
                    gps_data.x = x_local;
                    gps_data.y = y_local;
                    gps_data.z = z_local;
                    gps_data_.push_back(gps_data);

                    // 输出信息到终端
                    RCLCPP_INFO(this->get_logger(), "Position - Alt: %.2f m, Lon: %.6f°, Lat: %.6f° (local: %.2f, %.2f, %.2f)",
                                alt, lon, lat, x_local, y_local, z_local);
                }
                first_line = false;
                prev_time = time_str;
            }
        }

        RCLCPP_INFO(this->get_logger(), "Loaded %lu trajectory points", trajectory_poses_.size());
    }

    void publishTrajectory()
    {
        auto now = this->now();

        // 发布Path消息
        nav_msgs::msg::Path path;
        path.header.stamp = now;
        path.header.frame_id = "map";
        path.poses = trajectory_poses_;
        path_pub_->publish(path);

        // 发布轨迹Marker消息
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = "map";
        marker.header.stamp = now;
        marker.ns = "trajectory";
        marker.id = 0;
        marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.scale.x = 0.1; // 线宽
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;

        // 添加轨迹点
        for (const auto &gps : gps_data_)
        {
            geometry_msgs::msg::Point point;
            point.x = gps.x;
            point.y = gps.y;
            point.z = gps.z;
            marker.points.push_back(point);
        }

        marker_pub_->publish(marker);

        // 发布高度Marker
        visualization_msgs::msg::Marker height_marker;
        height_marker.header.frame_id = "map";
        height_marker.header.stamp = now;
        height_marker.ns = "height";
        height_marker.id = 1;
        height_marker.type = visualization_msgs::msg::Marker::LINE_LIST;
        height_marker.action = visualization_msgs::msg::Marker::ADD;
        height_marker.scale.x = 0.05; // 线宽
        height_marker.color.g = 1.0;  // 绿色
        height_marker.color.a = 0.5;

        // 为每个点添加高度线
        for (const auto &gps : gps_data_)
        {
            geometry_msgs::msg::Point base, top;
            base.x = gps.x;
            base.y = gps.y;
            base.z = 0;

            top.x = gps.x;
            top.y = gps.y;
            top.z = gps.z;

            height_marker.points.push_back(base);
            height_marker.points.push_back(top);
        }

        marker_pub_->publish(height_marker);
    }

    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::vector<geometry_msgs::msg::PoseStamped> trajectory_poses_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TrajectoryVisualization>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
