cmake_minimum_required(VERSION 3.8)

set(package_name "IMU_pkg")  # 从 shell 变量传入
set(node_name "IMU_node")    # 从 shell 变量传入

project(IMU_pkg)
# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2 REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(asio_cmake_module REQUIRED)  
find_package(serial_driver REQUIRED)  
find_package(std_msgs REQUIRED)

add_executable(IMU_node src/IMU_node.cpp src/IMUSerialReader.cpp)
ament_target_dependencies(IMU_node 
  rclcpp
  std_msgs
  sensor_msgs
  tf2_ros
  tf2
  geometry_msgs
  serial_driver  
)

install(TARGETS
  IMU_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
