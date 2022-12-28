# Step12.打包Debug和Release

注意：此示例适用于单配置生成器，不适用于多配置生成器（例如Visual Studio）。

默认情况下，CMake的模型是构建目录只包含一个配置，无论是Debug、Release、MinSizeRel还是RelWithDebInfo。但是，可以将CPack设置为捆绑多个构建目录，并构建包含同一项目的多个配置的包。

首先，我们希望确保调试版本和发布版本对将要安装的库使用不同的名称。让我们使用d作为调试库的后缀。

在顶层CMake的开始位置设置`CMAKE_DEBUG_POSTFIX`

```cmake
set(CMAKE_DEBUG_POSTFIX d)

add_library(tutorial_compiler_flags INTERFACE)
```

设置`DEBUG_POSTFIX`属性

```cmake
add_executable(Tutorial tutorial.cxx)
set_target_properties(Tutorial PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)
```

给MathFunctions库添加版版本号，设置`VERSION`和`SOVERSION`

```cmake
set_property(TARGET MathFunctions PROPERTY VERSION "1.0.0")
set_property(TARGET MathFunctions PROPERTY SOVERSION "1")ake

```

分别创建Release和Debug文件夹，分离两种模式的项目文件

现在我们需要设置调试和发布版本。我们可以使用`CMAKE_BUILD_TYPE`设置配置类型：

```shell
cmake -S . -B build/Debug -DCMAKE_BUILD_TYPE=Debug
```

现在调试和发布版本都已完成，我们可以使用自定义配置文件将两个版本打包到一个版本中。
在Step12目录中，创建一个名为`MultiCPackConfig.cmake`的文件。在该文件中，首先包含cmake可执行文件创建的默认配置文件。

接下来，使用`CPACK_INSTALL_CMAKE_PROJECTS`变量指定要安装的项目。在这种情况下，我们希望同时安装调试和发布。

MultiCPackConfig.cmake
```cmake
include("release/CPackConfig.cmake")

set(CPACK_INSTALL_CMAKE_PROJECTS
    "debug;Tutorial;ALL;/"
    "release;Tutorial;ALL;/"
)
```

在Step12目录中，运行cpack，使用config选项指定自定义配置文件：

```shell
cpack --config MultiCPackConfig.cmake
```


