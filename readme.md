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
## WSL2 安装 XRDP 服务
- 
    ```bash
    sudo apt update
    sudo apt install -y xfce4 xrdp
    sudo apt install lightdm
    sudo dpkg-reconfigure lightdm
    sudo vim /etc/xrdp/xrdp.ini #修改端口
    vim ~/.xsession
    # 写入下面内容(就一行)
    xfce4-session
    sudo /etc/init.d/xrdp start
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

## 问题记录
1. 在使用自定义接口消息时，第一次编译不通过，但是第二次编译通过 </br>
    - 原因：编译主程序时，没有编译自定义消息类型，所以编译不通过，但是第二次编译时，编译了自定义消息类型，所以编译通过
    - 解决方法：在```CMakeLists.txt``` 中 ```add_executable```后添加如下内容，确保了消息生成在节点编译之前完成建立了正确的构建依赖关系
    ``` CMakeLists.txt
    # 3. 添加对生成的消息的依赖
    rosidl_get_typesupport_target(cpp_typesupport_target
    ${package_name} "rosidl_typesupport_cpp")

    # 4. 确保在构建节点之前生成消息
    rosidl_target_interfaces(${node_name} ${package_name}
    "rosidl_typesupport_cpp")
    ```
