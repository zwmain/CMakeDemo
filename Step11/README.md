# 添加导出配置

在教程的安装和测试部分，我们为CMake添加了安装项目库和标题的功能。
在打包安装程序部分，我们添加了打包此信息的功能，以便将其分发给其他人。

下一步是添加必要的信息，以便其他CMake项目可以使用我们的项目，无论是从构建目录、本地安装还是打包时。

第一步是更新`install(TARGETS)`命令，不仅指定`DESTINATION`，还指定`EXPORT`。
`EXPORT`关键字生成一个CMake文件，其中包含从安装树导入安装命令中列出的所有目标的代码。
因此，让我们继续通过更新`MathFunctions/CMakeLists.txt`中的`install`命令来显式导出MathFunctions库，如下所示：

```cmake
set(installable_libs MathFunctions tutorial_compiler_flags)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs}
        EXPORT MathFunctionsTargets
        DESTINATION lib)
# install include headers
install(FILES MathFunctions.h DESTINATION include)

```

现在我们已经导出了MathFunctions，我们还需要显式安装生成的`MathFunctionsTargets.cmake`文件。
这是通过将以下内容添加到顶级CMakeLists的尾部来完成的：

```cmake
install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

```

此时，您应该尝试运行CMake。如果一切设置正确，您将看到CMake将生成如下错误：

```shell
Target "MathFunctions" INTERFACE_INCLUDE_DIRECTORIES property contains
path:

  "/Users/robert/Documents/CMakeClass/Tutorial/Step11/MathFunctions"

which is prefixed in the source directory.

```

CMake想说的是，在生成导出信息的过程中，它将导出一条与当前机器固有关联的路径，而在其他机器上则无效。
解决方案是更新MathFunctions的`target_include_directories()`，以了解在构建目录和安装包中使用时，它需要不同的INTERFACE位置。
这意味着将MathFunctions的`target_include_directories()`调用转换为如下所示：

```cmake
target_include_directories(MathFunctions
                           INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:include>
)

```

更新后，我们可以重新运行CMake并验证它是否不再发出警告。

此时，我们已经让CMake正确打包了所需的目标信息，但我们仍然需要生成`MathFunctionsConfig.CMake`，以便CMake的`find_package()`命令可以找到我们的项目。
因此，让我们继续向项目的顶层添加一个名为`Config.cmake.in`的新文件，其中包含以下内容：

```cmake
@PACKAGE_INIT@

include ( "${CMAKE_CURRENT_LIST_DIR}/MathFunctionsTargets.cmake" )

```

然后，要正确配置和安装该文件，请在顶级CMakeLists.txt的底部添加以下内容：

```cmake
install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

include(CMakePackageConfigHelpers)

```

接下来，我们执行`configure_package_config_file()`。
此命令将配置提供的文件，但与标准的`configure_file()`方式有一些特定的区别。
要正确使用此功能，除了所需的内容外，输入文件还应有一行文本`@PACKAGE_INIT@`。
该变量将被一块代码替换，该代码将设置值转换为相对路径。这些新值可以用相同的名称引用，但前缀为`PACKAGE_`。

```cmake
install(EXPORT MathFunctionsTargets
  FILE MathFunctionsTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

```

接下来是`write_basic_package_version_file()`。
此命令将写入一个文件，该文件由`find_package`使用，用于记录所需包的版本和兼容性。
这里，我们使用`Tutorial_VERSION_*`变量，并表示它与`AnyNewerVersion`兼容，这表示此版本或任何更高版本与请求的版本兼容。

最后，设置要安装的两个生成文件：

```cmake
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfigVersion.cmake
  DESTINATION lib/cmake/MathFunctions
)

```

此时，我们已经为我们的项目生成了一个可重定位的CMakeConfiguration，该配置可以在项目安装或打包后使用。如果我们希望我们的项目也可以从构建目录中使用，我们只需在顶层CMakeLists.txt的底部添加以下内容：

```cmake
export(EXPORT MathFunctionsTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsTargets.cmake"
)

```

通过此导出调用，我们现在生成了`Targets.cmake`，允许其他项目使用构建目录中配置的`MathFunctionsConfig.cmake`，而无需安装它。

## 小结

导出配置文件，首先需要被导出的库，在`install(TARGETS)`的时候，使用`EXPORT`关键字生成`Targets`文件

如果库名是MathFunctions，那么`EXPORT`关键字后面就是MathFunctionsTargets

```cmake
install(TARGETS ${installable_libs}
        EXPORT MathFunctionsTargets # 这一行为额外增加的
        DESTINATION lib)

```

在上级CMake中，安装这个导出文件，与安装普通文件相比，多了`EXPORT`关键字

```cmake
install(EXPORT MathFunctionsTargets   # 这里需要指定EXPORT和目标
  FILE MathFunctionsTargets.cmake     # 明确目标文件是XXXTargets.cmake
  DESTINATION lib/cmake/MathFunctions
)

```

准备一个`Config.cmake.in`文件，为生成XXXConfig.cmake做准备

填入如下内容，这里`CMAKE_CURRENT_LIST_DIR`是当前正在处理的列表文件的完整目录。

参考：[CMAKE_CURRENT_LIST_DIR](https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_LIST_DIR.html)

```
@PACKAGE_INIT@

include ( "${CMAKE_CURRENT_LIST_DIR}/MathFunctionsTargets.cmake" )

```

包含`CMakePackageConfigHelpers`模块，使用`configure_package_config_file`生成Config文件

```cmake
include(CMakePackageConfigHelpers)
# 生成包含导出的配置文件
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

```

借助`write_basic_package_version_file`生成Version文件

```cmake
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfigVersion.cmake"
  VERSION "${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

```

最后安装Config和Version文件

```cmake
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsConfigVersion.cmake
  DESTINATION lib/cmake/MathFunctions
)

```




