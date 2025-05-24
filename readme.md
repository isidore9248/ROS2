# ROS2 
## 环境： WSL2 +  Ubuntu22.04 +  ROS2 Humble

## WSL2环境迁移

1. 查看wsl状态
    ```bash
    wsl -l -v
    ```
2. 关闭运行状态
    ```bash
    wsl --shutdown
    ```
3. 导出镜像
     ```bash
    wsl --export Ubuntu-22.04 D:\wsl\fileStore\ubuntu22.04_ROS2\ubuntu22.04.tar        
    ```
4. 删除镜像
     ```bash
    wsl --unregister Ubuntu-22.04     
    ```
5. 导入镜像
      ```bash
    wsl --import Ubuntu-22.04 D:\wsl\fileStore\ubuntu22.04_ROS2 D:\wsl\fileStore\ubuntu22.04_ROS2\ubuntu22.04.tar --version 2      
    ```
6. 切换默认wsl2版本
    ```bash
    wsl --set-default Ubuntu-22.04
    ```
##  ROS2安装

- 使用小鱼ROS一键安装
    ```bash
    wget http://fishros.com/install -O fishros && . fishros
    ```
## 自动化脚本文件(autobash/下)
- ```build_ros2_ws.sh```
    </br>ros2 的编译脚本,放在ws下,执行可自动编译
- ```create_ros2_ws.sh```
    </br>工作空间创建脚本,执行后可在当前目录下创建ws工作空间
- ```source.sh```
    </br>刷新环境变量脚本 执行时使用```source```命令执行，不要用```./```的形式
- ```note.txt```
    </br>学习笔记