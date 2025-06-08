#!/bin/bash

export DISPLAY=:0

# # 先执行source命令并检查是否成功
# source /home/ros2/ROS2Folder/Event/install/setup.bash
# if [ $? -ne 0 ]; then
#     echo "Error: Failed to source setup.bash"
#     exit 1
# fi

# * source /home/ros2/ROS2Folder/Event/trajectory_ws/install/setup.bash
# 打开第一个终端窗口执行ros2 launch
gnome-terminal -- bash -c " ros2 launch trajectory_visualization visualization.launch.py; exec bash"

# 打开第二个终端窗口执行python绘图脚本
gnome-terminal -- bash -c "cd /home/ros2/ROS2Folder/Event && python3 plot_gps.py; exec bash"