# 打包安装程序

接下来假设我们想将我们的项目分发给其他人，以便他们可以使用它。我们想在各种平台上提供二进制和源代码分发。
这与我们之前在“安装和测试”中所做的安装略有不同，我们在安装从源代码构建的二进制文件。
在本例中，我们将构建支持二进制安装和包管理功能的安装包。
为此，我们将使用CPack创建特定于平台的安装程序。具体来说，我们需要在顶级CMakeLists文件的底部添加几行。

```cmake
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

```

这就是它的全部。我们从include`InstallRequiredSystemLibraries`开始。该模块将包括项目当前平台所需的任何运行库。
接下来，我们将一些CPack变量设置为存储此项目的许可证和版本信息的位置。
版本信息已在本教程的前面设置，`License.txt`已包含在本步骤的顶级源目录中。
`CPACK_SOURCE_GENERATOR`变量为源包选择文件格式。

最后，我们include`CPack`模块，它将使用这些变量和当前系统的一些其他属性来设置安装程序。

下一步是以通常的方式构建项目，然后运行cpack命令。要构建二进制分发，请从二进制目录运行：

```shell
cpack
```

要指定生成器，请使用`-G`选项。对于多配置版本，请使用`-C`指定配置。例如：

```shell
cpack -G ZIP -C Debug
```

有关可用生成器的列表，请参阅[cpackgenerators](https://cmake.org/cmake/help/latest/manual/cpack-generators.7.html)或调用`cpack --help`。ZIP这样的[归档生成器](https://cmake.org/cmake/help/latest/cpack_gen/archive.html#cpack_gen:CPack%20Archive%20Generator)会创建所有已安装文件的压缩归档。

要创建完整源代码树的存档，请键入：

```shell
cpack --config CPackSourceConfig.cmake
```

或者运行`make package`命令，右键单击IDE中的`Package`目标和`Build Project`。

运行二进制目录中的安装程序。然后运行已安装的可执行文件并验证其是否正常工作。

最终打包结果相当于，先`install`再打包




