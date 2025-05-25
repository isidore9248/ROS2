cmake_minimum_required(VERSION 3.8)

set(package_name "topic_cpp")
set(node_name "topicpublish")
set(node_name2 "topicsubscribe")

project(${package_name})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(${node_name} src/topicpublish.cpp )  # 正确：直接引用 CMake 变量
ament_target_dependencies(${node_name} rclcpp std_msgs)

add_executable(${node_name2} src/topicsubscribe.cpp )  # 正确：直接引用 CMake 变量
ament_target_dependencies(${node_name2} rclcpp std_msgs)

install(TARGETS
  ${node_name}
  ${node_name2}
  DESTINATION lib/${PROJECT_NAME})

ament_package()
