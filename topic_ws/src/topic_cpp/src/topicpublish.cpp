#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

//*自定义节点类
class TopicPublish : public rclcpp::Node
{
public:
    TopicPublish() : Node("topicpublish")
    {
        RCLCPP_INFO(this->get_logger(), "Hello from ROS2!");
        //* 1创建发布者
        //? 话题名称 队列深度
        //? 返回发布对象指针
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        //! 终端命令查看消息 ros2 topic echo /topic (/topic为话题名称,即create_publisher中的topic)
        //* 2创建定时器
        //? 定时器时间 定时器回调函数
        timer_ = this->create_wall_timer
        (
            std::chrono::milliseconds(1000),
            [this]() -> void
            {
                auto message = std_msgs::msg::String();
                message.data = "Hello, world!";
                RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
                publisher_->publish(message);
            }
        );
    }
    ~TopicPublish()
    {
        RCLCPP_INFO(this->get_logger(), "Goodbye from ROS2!");
    }

private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
    //* 1初始化
    rclcpp::init(argc, argv);
    //* 2创建节点
    auto node = std::make_shared<TopicPublish>();
    
    rclcpp::spin(node);
    
    //* 资源释放
    rclcpp::shutdown();

    return 0;
}
