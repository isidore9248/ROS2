#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <nav_msgs/msg/path.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

class TrajectoryVisualization : public rclcpp::Node {
public:
    TrajectoryVisualization() : Node("trajectory_visualization") {
        // 创建marker发布者
        marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("trajectory_marker", 10);
        path_pub_ = this->create_publisher<nav_msgs::msg::Path>("trajectory_path", 10);

        // 读取数据并处理
        readDataFile();
        
        // 定时发布数据
        timer_ = this->create_wall_timer(100ms, std::bind(&TrajectoryVisualization::publishTrajectory, this));
    }

private:
    void readDataFile() {
        std::ifstream file("/home/ros2/ROS2Folder/Event/data.txt");
        if (!file.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open data file");
            return;
        }
        std::string line;
        double x = 0.0, y = 0.0;  // 位置
        double vx = 0.0, vy = 0.0;  // 速度
        double dt = 0.0;  // 时间间隔
        std::string prev_time;
        bool first_line = true;

        // 跳过标题行
        std::getline(file, line);
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string address, time;
            double ax, ay, wz;
            
            // 读取数据
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(iss, token, '\t')) {
                tokens.push_back(token);
            }

            if (tokens.size() >= 9) {
                time = tokens[1];
                ax = std::stod(tokens[2]);  // ax(g)
                ay = std::stod(tokens[3]);  // ay(g)
                wz = std::stod(tokens[8]);  // wz(deg/s)

                // 转换加速度单位从g到m/s^2
                ax *= 9.81;
                ay *= 9.81;
                
                if (!first_line) {
                    // 计算时间间隔 (简单处理，仅获取毫秒差值)
                    dt = 0.1;  // 假设采样率为10Hz
                    
                    // 使用加速度积分得到速度
                    vx += ax * dt;
                    vy += ay * dt;
                    
                    // 使用速度积分得到位置
                    x += vx * dt;
                    y += vy * dt;
                    
                    // 存储轨迹点
                    geometry_msgs::msg::PoseStamped pose;
                    pose.pose.position.x = x;
                    pose.pose.position.y = y;
                    pose.pose.position.z = 0.0;
                    
                    // 根据角速度设置方向
                    tf2::Quaternion q;
                    q.setRPY(0, 0, wz * M_PI / 180.0);  // 将角速度转换为弧度
                    pose.pose.orientation.x = q.x();
                    pose.pose.orientation.y = q.y();
                    pose.pose.orientation.z = q.z();
                    pose.pose.orientation.w = q.w();
                    
                    trajectory_poses_.push_back(pose);
                }
                first_line = false;
                prev_time = time;
            }
        }
        
        RCLCPP_INFO(this->get_logger(), "Loaded %lu trajectory points", trajectory_poses_.size());
    }

    void publishTrajectory() {
        auto now = this->now();
        
        // 发布Path消息
        nav_msgs::msg::Path path;
        path.header.stamp = now;
        path.header.frame_id = "map";
        path.poses = trajectory_poses_;
        path_pub_->publish(path);
        
        // 发布Marker消息
        visualization_msgs::msg::Marker marker;
        marker.header.frame_id = "map";
        marker.header.stamp = now;
        marker.ns = "trajectory";
        marker.id = 0;
        marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.scale.x = 0.1;  // 线宽
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;
        
        for (const auto& pose : trajectory_poses_) {
            geometry_msgs::msg::Point point;
            point.x = pose.pose.position.x;
            point.y = pose.pose.position.y;
            point.z = pose.pose.position.z;
            marker.points.push_back(point);
        }
        
        marker_pub_->publish(marker);
    }

    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::vector<geometry_msgs::msg::PoseStamped> trajectory_poses_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TrajectoryVisualization>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
