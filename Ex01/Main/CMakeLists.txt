add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Utils)

# 安装 ---

# 设置可执行文件的RPATH，以寻找动态库
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib")

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "bin"
)

# 安装 ---

