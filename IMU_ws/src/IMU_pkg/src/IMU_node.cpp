#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

#include "../include/IMU_pkg/IMUSerialReader.h"

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<IMUSerialReader>("/dev/ttyS1",115200);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}