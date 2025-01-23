#!/bin/bash

# 检查build目录是否存在，如果不存在则创建
if [ ! -d "build" ]; then
    echo "Create build directory..."
    mkdir build
fi

# 进入build目录
cd build

# 判断操作系统并选择合适的构建工具
if [ "$(uname)" == "Darwin" ] || [ "$(uname)" == "Linux" ]; then
    # 如果是Linux或macOS平台，使用CMake生成Makefile
    echo "Running CMake on Linux/macOS..."
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make -j8
elif [ -n "$WINDIR" ]; then
    # 如果是Windows平台，使用Ninja生成构建文件
    echo "Running Ninja on Windows..."
    cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ..
    # 使用 Ninja 构建项目
    echo "Compiling project using Ninja..."
    ninja -j8
else
    echo "Unsupported platform!"
    exit 1
fi

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
else
    echo "Compilation failed."
fi

# 返回到原始目录
cd ..