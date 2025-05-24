#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class TopicSubscribe : public rclcpp::Node
{
public:
    TopicSubscribe() : Node("topicsubscribe")
    {
        RCLCPP_INFO(this->get_logger(), "Hello from ROS2!");
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "topic",
            10,
            std::bind(&TopicSubscribe::topic_callback, this, std::placeholders::_1)
        );
    }

    ~TopicSubscribe()
    {
        RCLCPP_INFO(this->get_logger(), "Goodbye from ROS2!");
    }

private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        static int count = 0;
        count++;
        RCLCPP_INFO(this->get_logger(), "I heard: '%s', count = %d", msg->data.c_str(), count);
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TopicSubscribe>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}