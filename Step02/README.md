# 添加一个库

## 练习1：创建一个库

想要添加库，使用`add_library()`命令并指定生成该库的源文件

```cmake
add_library(MathFunctions mysqrt.cxx)

```

相对于把所有文件都放在同一个文件夹下，CMake更推荐使用多个文件夹组织项目。

在这种情况下，需要为库创建一个子文件夹。在子文件夹内创建一个`CMakeLists.txt`管理文件夹中的源代码。在上一级文件夹下的`CMakeLists.txt`中使用`add_subdirectory()`添加文件夹。

库一旦被创建，他就通过`target_include_directories()`和`target_link_libraries()`连接到可执行目标

## 联系2：可选的库

可以通过`option()`命令让库成为可选的。这为用户提供了一个变量，用户可以在配置cmake构建时配置该变量。此设置将存储在缓存中，因此用户无需在每次在构建目录上运行CMake时设置该值。

使用`list()`函数在CMake的`if()`语句块中生成库和头文件列表，在指定目标的库和头文件目录时，使用前面生成的变量而不是显示写出库名称和文件路径

`option()`命令要在`configure`前面，因为要在`TutorialConfig.h.in`中使用`#cmakedefine`，该指令会用到`USE_MYMATH`

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)


if(USE_MYMATH)
    add_subdirectory(MathFunctions)
    list(APPEND EXTRA_LIBS MathFunctions)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

```

## 源码

项目结构：

```txt
.
├── CMakeLists.txt
├── MathFunctions
│   ├── CMakeLists.txt
│   ├── MathFunctions.h
│   └── mysqrt.cxx
├── README.md
├── TutorialConfig.h.in
└── tutorial.cxx

```





