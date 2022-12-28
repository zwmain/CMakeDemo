# 为测试仪表盘增加支持

添加对将测试结果提交到仪表板的支持很简单。
我们已经在测试支持中为我们的项目定义了许多测试。
现在我们只需要运行这些测试并将其提交到仪表板。
为了包括对仪表板的支持，我们在顶级CMakeLists.txt中包含CTest模块。

替换

```cmake
enable_testing()
```

为：

```cmake
include(CTest)
```

CTest模块会自动调用`enable_testing()`，所以可以移除。

我们还需要获取一个`CTestConfig.cmake`文件，将其放置在顶级目录中，在那里我们可以向CTest指定有关项目的信息。它包含：

- 项目名称
- 项目“Nightly”开始时间
  - 该项目开始24小时“一天”的时间。
- 将发送提交的生成文档的CDash实例的URL

此目录中已为您提供了一个。它通常会从CDash实例上的项目设置页面下载，该实例将托管并显示测试结果。从CDash下载后，不应在本地修改文件。

```cmake
set(CTEST_PROJECT_NAME "CMakeTutorial")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=CMakeTutorial")
set(CTEST_DROP_SITE_CDASH TRUE)
```

ctest命令在运行时将读入该文件。要创建一个简单的仪表板，您可以运行cmake命令或cmake-gui来配置项目，但不要构建它。相反，将目录更改为二进制树，然后运行：

```shell
ctest [-VV] -D Experimental
```

如果是Visual Studio，记得加`-C Debug`。或者，从IDE构建实验目标。

ctest可执行文件将构建和测试项目，并将结果提交到Kitware的公共仪表板：[https://my.cdash.org/index.php?project=CMakeTutorial](https://my.cdash.org/index.php?project=CMakeTutorial)

数据被提交到上面那个公共网站，打开可以看到多出来一个结果





