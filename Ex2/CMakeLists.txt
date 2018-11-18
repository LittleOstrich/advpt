cmake_minimum_required(VERSION 2.8)

project(Ex2)

file(GLOB src
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} "main.cpp" ${src})
