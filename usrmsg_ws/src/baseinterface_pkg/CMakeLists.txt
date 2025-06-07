cmake_minimum_required(VERSION 3.8)

set(package_name "baseinterface_pkg")  # 从 shell 变量传入
set(node_name "baseinterface_node")    # 从 shell 变量传入

project(${package_name})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

find_package(rosidl_generator_cpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# * 给接口文件生成源代码
rosidl_generate_interfaces(
${package_name}
"msg/Usrmsg.msg"
)

add_executable(${node_name} src/${node_name}.cpp)

# 3. 添加对生成的消息的依赖
rosidl_get_typesupport_target(cpp_typesupport_target
  ${package_name} "rosidl_typesupport_cpp")

# 4. 确保在构建节点之前生成消息
rosidl_target_interfaces(${node_name} ${package_name}
  "rosidl_typesupport_cpp")

  
target_include_directories(${node_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(${node_name} PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  ${node_name}
  "rclcpp"
  "std_msgs"
)


install(TARGETS ${node_name}
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
