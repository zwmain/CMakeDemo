cmake_minimum_required(VERSION 3.5)

project(main)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE lib1 lib2)
