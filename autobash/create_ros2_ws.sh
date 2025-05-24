#!/bin/bash

# 获取脚本所在目录
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

# 默认值设置
DEFAULT_WS_NAME="ros2_ws"
DEFAULT_PKG_NAME="my_cpp_pkg"
DEFAULT_NODE_NAME="my_node"
DEFAULT_DEPENDENCIES="rclcpp std_msgs"

# 显示默认值
echo -e "\n默认配置:"
echo -e "工作空间名称: \033[33m$DEFAULT_WS_NAME\033[0m"
echo -e "功能包名称: \033[33m$DEFAULT_PKG_NAME\033[0m"
echo -e "节点名称: \033[33m$DEFAULT_NODE_NAME\033[0m"
echo -e "依赖项: \033[33m$DEFAULT_DEPENDENCIES\033[0m"

# 获取用户输入
read -p "请输入工作空间名称 (直接回车使用默认值): " ws_name
ws_name=${ws_name:-$DEFAULT_WS_NAME}

read -p "请输入功能包名称 (直接回车使用默认值): " pkg_name
pkg_name=${pkg_name:-$DEFAULT_PKG_NAME}

read -p "请输入节点名称 (直接回车使用默认值): " node_name
node_name=${node_name:-$DEFAULT_NODE_NAME}

read -p "请输入依赖项 (直接回车使用默认值): " dependencies
dependencies=${dependencies:-$DEFAULT_DEPENDENCIES}

# 检查ROS2环境
check_ros2_environment() {
    echo "检查ROS2环境..."
    if ! command -v ros2 &> /dev/null; then
        echo "未找到ros2命令，尝试source环境..."
        if [ -f "/opt/ros/humble/setup.bash" ]; then
            source /opt/ros/humble/setup.bash
            if ! command -v ros2 &> /dev/null; then
                echo "错误：ROS2环境配置失败"
                return 1
            fi
        else
            echo "错误：找不到ROS2安装目录"
            return 1
        fi
    fi
    return 0
}

# 检查依赖
check_dependencies() {
    local missing_deps=()
    
    if ! command -v colcon &> /dev/null; then
        missing_deps+=("python3-colcon-common-extensions")
    fi
    
    if ! command -v cmake &> /dev/null; then
        missing_deps+=("cmake")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        echo "缺少以下依赖："
        printf '%s\n' "${missing_deps[@]}"
        echo "请使用以下命令安装："
        echo "sudo apt install ${missing_deps[*]}"
        return 1
    fi
    return 0
}

# 检查环境
check_ros2_environment || exit 1
check_dependencies || exit 1

# 创建工作空间
echo "创建工作空间目录结构..."
mkdir -p "$SCRIPT_DIR/${ws_name}/src"
cd "$SCRIPT_DIR/${ws_name}/src"

# 创建功能包
echo "创建ROS2功能包..."
ros2 pkg create --build-type ament_cmake --dependencies ${dependencies} \
    --node-name ${node_name} ${pkg_name}

# 修改package.xml
echo "配置package.xml..."
cat > ${pkg_name}/package.xml << EOL
<?xml version="1.0"?>
<?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
<package format="3">
  <name>${pkg_name}</name>
  <version>0.0.1</version>
  <description>ROS2 C++ package</description>
  <maintainer email="user@todo.todo">user</maintainer>
  <license>MIT-0</license>
  
  <buildtool_depend>ament_cmake</buildtool_depend>
  
  <build_depend>rclcpp</build_depend>
  <build_depend>std_msgs</build_depend>
  
  <exec_depend>rclcpp</exec_depend>
  <exec_depend>std_msgs</exec_depend>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
EOL

# 修改CMakeLists.txt
echo "配置CMakeLists.txt..."
cat > ${pkg_name}/CMakeLists.txt << EOL
cmake_minimum_required(VERSION 3.8)

set(package_name "${pkg_name}")  # 从 shell 变量传入
set(node_name "${node_name}")    # 从 shell 变量传入

project(${pkg_name})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(${node_name} src/${node_name}.cpp)  # 正确：直接引用 CMake 变量
ament_target_dependencies(${node_name} rclcpp std_msgs)

install(TARGETS
  ${node_name}
  DESTINATION lib/\${PROJECT_NAME})

ament_package()
EOL

# 初始构建
cd ..
echo "初始构建工作空间..."
colcon build --symlink-install

# 输出结果
echo -e "\n\033[32m[成功] ROS2工作空间已创建！\033[0m"
echo -e "工作空间路径: \033[33m$(pwd)\033[0m"
echo -e "使用以下命令加载环境:"
echo -e "\033[36msource install/setup.bash\033[0m"
echo -e "运行节点命令:"
echo -e "\033[36mros2 run ${pkg_name} ${node_name}\033[0m"