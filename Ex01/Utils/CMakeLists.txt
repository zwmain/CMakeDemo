project(Utils)

add_library(
    ${PROJECT_NAME} SHARED
    "inc/MyMath.h"
    "src/MyMath.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

# 安装 ---

install(
    TARGETS ${PROJECT_NAME}
    EXPORT "UtilsTargets"
    DESTINATION "lib"
)

install(
    EXPORT "UtilsTargets"
    FILE "UtilsTargets.cmake"
    DESTINATION "cmake"
)

include(CMakePackageConfigHelpers)
# 生成Config文件
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsConfig.cmake"
    INSTALL_DESTINATION "cmake"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# 生成Version文件
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsConfigVersion.cmake"
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)
# 安装上面两个文件
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsConfigVersion.cmake"
    DESTINATION "cmake"
)

install(
    FILES "inc/MyMath.h"
    DESTINATION "include"
)

# 安装 ---

