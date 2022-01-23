cmake_minimum_required(VERSION 3.5)

project(shared-library)

add_library(hello_lib SHARED src/hello.cpp)

target_include_directories(hello_lib PUBLIC ${PROJECT_SOURCE_DIR}/inc)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE hello_lib)
