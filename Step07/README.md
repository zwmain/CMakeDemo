# 添加系统自检

让我们考虑向项目中添加一些代码，这些代码取决于目标平台可能不具备的功能。
对于本例，我们将添加一些代码，这些代码取决于目标平台是否具有`log`和`exp`函数。
当然，几乎每个平台都有这些功能，但本教程假设它们并不常见。

如果平台有`log`和`exp`，那么我们将使用它们来计算`mysqrt`函数中的平方根。
我们首先在`MathFunctions/CMakeLists.txt`中使用`CheckCXXSourceCompiles`模块测试这些函数的可用性。

在调用`target_include_directories()`之后，将`log`和`exp`的检查添加到`MathFunctions/CMakeLists.txt`：

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

# link our compiler flags interface library
target_link_libraries(MathFunctions tutorial_compiler_flags)

# does this system provide the log and exp functions?
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::log(1.0);
    return 0;
  }
" HAVE_LOG)
check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::exp(1.0);
    return 0;
  }
" HAVE_EXP)

```

如果可用，请使用`target_compile_definitions()`将`HAVE_LOG`和`HAVE_EXP`指定为`PRIVATE`编译定义。

```cmake
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()

```

如果系统上有`log`和`exp`，那么我们将使用它们来计算`mysqrt`函数中的平方根。将以下代码添加到`MathFunctions/mysqrt.cxx`中的`mysqrt`函数（在返回结果之前不要忘记#endif！）：

```c++
#if defined(HAVE_LOG) && defined(HAVE_EXP)
  double result = std::exp(std::log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << " to be " << result
            << " using log and exp" << std::endl;
#else
  double result = x;

```

在此之前需要包含`cmath`头文件






