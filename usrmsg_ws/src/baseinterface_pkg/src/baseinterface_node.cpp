#include "rclcpp/rclcpp.hpp"
#include "baseinterface_pkg/msg/usrmsg.hpp"  // 修正了自定义消息头文件路径

int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;

  printf("hello world baseinterface_pkg package\n");
  return 0;
}