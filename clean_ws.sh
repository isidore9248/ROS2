#!/bin/bash

# 获取脚本所在目录
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

# 定义颜色
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}开始清理工作空间...${NC}"

# 检查并清理 build 目录
if [ -d "build" ]; then
    echo "清理 build 目录..."
    rm -rf build
fi

# 检查并清理 install 目录
# if [ -d "install" ]; then
#     echo "清理 install 目录..."
#     rm -rf install
# fi

# 检查并清理 log 目录
if [ -d "log" ]; then
    echo "清理 log 目录..."
    rm -rf log
fi

# 清理 CMake 缓存文件
find . -name "CMakeCache.txt" -exec rm {} \;
find . -name "CMakeFiles" -exec rm -rf {} + 2>/dev/null || true

# # 清理 Python 缓存文件
# find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
# find . -name "*.pyc" -exec rm {} \;

echo -e "${GREEN}清理完成!${NC}"
echo "你现在可以重新构建工作空间:"
echo -e "${YELLOW}colcon build --symlink-install${NC}"