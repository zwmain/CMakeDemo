cmake_minimum_required(VERSION 3.5)

project(lib2)

add_library(${PROJECT_NAME} SHARED src/print-string.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)
