cmake_minimum_required(VERSION 3.15)
project(Ex VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/InsDir")

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory("Utils")
add_subdirectory("Main")

