#!/bin/bash

# 获取脚本所在目录
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

# 检查是否在工作空间目录下
if [ ! -d "src" ]; then
    echo "错误：当前目录不是ROS2工作空间目录！"
    echo "请在工作空间根目录下运行此脚本"
    exit 1
fi

# 询问是否使用 symlink-install
echo -e "\n是否使用符号链接安装(symlink-install)？"
echo -e "使用符号链接安装可以避免重复复制文件，适合开发阶段使用"
echo -n "是否使用？[y/N]: "
read use_symlink
if [[ $use_symlink =~ ^[Yy]$ ]]; then
    SYMLINK_OPT="--symlink-install"
else
    SYMLINK_OPT=""
fi

# 显示对话框
echo -e "\n请选择编译方式："
echo -e "1) 编译整个工作空间 (colcon build)"
echo -e "2) 编译指定功能包 (colcon build --packages-select)"
echo -n "请输入选项 [1/2]: "
read choice

case $choice in
    1)
        echo "编译整个工作空间..."
        colcon build $SYMLINK_OPT
        ;;
    2)
        # 获取src目录下的所有功能包
        echo -e "\n可用的功能包："
        ls src/
        echo -n "请输入要编译的功能包名称: "
        read package_name
        if [ -d "src/$package_name" ]; then
            echo "编译功能包 $package_name..."
            colcon build --packages-select $package_name $SYMLINK_OPT
        else
            echo "错误：功能包 $package_name 不存在！"
            exit 1
        fi
        ;;
    *)
        echo "无效的选项！"
        exit 1
        ;;
esac

if [ $? -eq 0 ]; then
    echo -e "\n\033[32m[成功] 编译完成！\033[0m"
    echo -e "请运行以下命令加载环境："
    echo -e "\033[36msource install/setup.bash\033[0m"
else
    echo -e "\n\033[31m[错误] 编译失败！\033[0m"
fi