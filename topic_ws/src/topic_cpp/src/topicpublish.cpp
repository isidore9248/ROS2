#include <cstdio>
#include "rclcpp/rclcpp.hpp"


int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("topicpublish");
    RCLCPP_INFO(node->get_logger(), "Hello from ROS2!");
    
    rclcpp::spin(node);
    rclcpp::shutdown();

  return 0;
}
