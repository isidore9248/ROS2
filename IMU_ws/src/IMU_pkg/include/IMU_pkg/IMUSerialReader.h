#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <serial_driver/serial_driver.hpp>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>

class IMUSerialReader : public rclcpp::Node
{
public:
    IMUSerialReader(std::string serial_port, int serial_baud,
                    std::string node_name = "imu_serial_reader");  // 默认参数保留在这里
    ~IMUSerialReader();

private:
    void readSerialData();
    void processSerialData();
    void publishIMUData(const std::vector<float> &values);

    // 串口相关
    std::unique_ptr<IoContext> io_context_;
    std::unique_ptr<drivers::serial_driver::SerialPortConfig> port_config_;
    std::unique_ptr<drivers::serial_driver::SerialDriver> serial_driver_;
    std::string port_;
    int baudrate_;
    std::string serial_buffer_;

    // ROS2相关
    std::string frame_id_;
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
};