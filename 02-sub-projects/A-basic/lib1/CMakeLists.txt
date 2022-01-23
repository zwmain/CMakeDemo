cmake_minimum_required(VERSION 3.5)

project(lib1)

add_library(${PROJECT_NAME} SHARED src/generate-string.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)
