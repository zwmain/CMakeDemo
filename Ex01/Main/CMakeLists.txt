add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Utils)

# 安装 ---

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "bin"
)

# 安装 ---

