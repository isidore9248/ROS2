#!/bin/bash

# 先加载ROS2基础环境
if [ -f "/opt/ros/humble/setup.bash" ]; then
    source /opt/ros/humble/setup.bash
else
    echo -e "\033[31m错误: ROS2环境文件不存在\033[0m"
    exit 1
fi

# 检查工作空间 setup.bash 文件是否存在
if [ ! -f "./install/setup.bash" ]; then
    echo -e "\033[31m错误: 工作空间setup.bash文件不存在\033[0m"
    exit 1
fi

# 执行工作空间的source命令
source ./install/setup.bash
