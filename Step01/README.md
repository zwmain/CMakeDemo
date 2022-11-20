# Step01.基本的起点

## 练习1.构建一个基础项目

任何一个CMake项目，都必须以`cmake_minimum_required()`命令指定最小版本开始。

每个项目应该在`cmake_minimum_required()`之后立即调用`project()`指定项目名称。

最后，使用`add_executable()`命令传入源码文件名和待生成的二进制文件名，告诉CMake生成一个可执行文件。

```cmake
cmake_minimum_required(VERSION 3.15)

project(Tutorial)

add_executable(Tutorial tutorial.cpp)

```

**构建并运行**

CMakeLists.txt文件所在的路径就是目标路径，`cmake 目标路径`就可以构建项目

在当前文件夹新建build文件夹，进入build文件夹，那么`..`就是目标文件夹，需要执行的命令就是`cmake ..`

CMake提供了很多选项可以使用：

```cmake
cmake -S 源码目录 -B 构建目录 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

到这里知识构建了项目，并没有编译

```cmake
cmake --build 构建文件夹 --config Debug
```

这样就可以编译了

## 练习2.指定C++版本

主要涉及两个变量：`CMAKE_CXX_STANDARD`和`CMAKE_CXX_STANDARD_REQUIRED`

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

变量必须在`add_executable`之前设置

## 练习3.添加版本号和配置头文件

创建`.in`文件

使用`configure_file()`命令指定配置文件

```cmake
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

设置头文件包含目录

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

添加配置文件内容，使用`@xxx@`引用CMake变量

```txt
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@

```
