cmake_minimum_required(VERSION 3.10)

project(TinyRaytracer VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(main src/main.cpp src/image.cpp src/raytracer.cpp)

