# Step10.选择静态库或共享库

在本节中，我们将展示如何使用`BUILD_SHARED_LIBS`变量来控制`add_library()`的默认行为，并允许控制如何构建没有显式类型（`STATIC`、`SHARED`、`MODULE`或`OBJECT`）的库。

为了实现这一点，我们需要将`BUILD_SHARED_LIBS`添加到顶级CMakeLists中。
我们使用`option()`命令，因为它允许用户选择值是`ON`还是`OFF`。

接下来，我们将重构MathFunctions，使其成为一个使用`mysqrt`或`sqrt`封装的真正的库，而不是要求调用代码执行此逻辑。
这也意味着`USE_MYMATH`不会控制构建MathFunctions，而是控制这个库的行为。

更改顶层CMakeLists为下面这样：

```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# configure a header file to pass the version number only
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

```

既然我们已经使MathFunctions始终被使用，我们就需要更新该库的逻辑。
因此，在`MathFunctions/CMakeLists.txt`中，我们需要创建一个SqrtLibrary，当启用`USE_MYMATH`时，它将有条件地构建和安装。
现在，由于这是一个教程，我们将明确要求静态构建SqrtLibrary。

最终`MathFunctions/CMakeLists.txt`看起来像这个样子：

```cmake
# add the library that runs
add_library(MathFunctions MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if(USE_MYMATH)

  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # first we add the executable that generates the table
  add_executable(MakeTable MakeTable.cxx)
  target_link_libraries(MakeTable PRIVATE tutorial_compiler_flags)

  # add the command to generate the source code
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    DEPENDS MakeTable
    )

  # library that just does sqrt
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              ${CMAKE_CURRENT_BINARY_DIR}/Table.h
              )

  # state that we depend on our binary dir to find Table.h
  target_include_directories(SqrtLibrary PRIVATE
                             ${CMAKE_CURRENT_BINARY_DIR}
                             )

  target_link_libraries(SqrtLibrary PUBLIC tutorial_compiler_flags)
  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

target_link_libraries(MathFunctions PUBLIC tutorial_compiler_flags)

# define the symbol stating we are using the declspec(dllexport) when
# building on windows
target_compile_definitions(MathFunctions PRIVATE "EXPORTING_MYMATH")

# install libs
set(installable_libs MathFunctions tutorial_compiler_flags)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs} DESTINATION lib)
# install include headers
install(FILES MathFunctions.h DESTINATION include)

```

接下来，更新`MathFunctions/mysqrt.cxx`以使用`mathfunctions`和`detail`名称空间：

```c++
#include <iostream>

#include "MathFunctions.h"

// include the generated table
#include "Table.h"

namespace mathfunctions {
namespace detail {
// a hack square root calculation using simple operations
double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  // use the table to help find an initial value
  double result = x;
  if (x >= 1 && x < 10) {
    std::cout << "Use the table to help find an initial value " << std::endl;
    result = sqrtTable[static_cast<int>(x)];
  }

  // do ten iterations
  for (int i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
  }

  return result;
}
}
}

```

我们还需要在`tutorial.cxx`中进行一些更改，以便它不再使用`USE_MYMATH`：

1. 总是包含`MathFunctions.h`
2. 总是使用`mathfunctions::sqrt`
3. 不要包含`cmath`

最后，更新`MathFunctions/MathFunctions.h`使用DLL导出宏定义

```c++
#if defined(_WIN32)
#  if defined(EXPORTING_MYMATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

namespace mathfunctions {
double DECLSPEC sqrt(double x);
}

```

此时，如果您构建了所有内容，您可能会注意到链接失败，因为我们将一个没有位置无关代码的静态库与一个具有位置无关代码库组合在一起。
解决方案是将SqrtLibrary的`POSITION_INDEPENDENT_CODE`目标属性显式设置为`True`，无论生成类型如何。

```cmake
  # state that SqrtLibrary need PIC when the default is shared libraries
  set_target_properties(SqrtLibrary PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

```

练习：我们修改了`MathFunctions.h`以使用dll导出定义。使用CMake文档，您可以找到一个帮助模块来简化这个过程吗？

参考[CSDN](https://blog.csdn.net/LaineGates/article/details/83899961)

使用`GENERATE_EXPORT_HEADER`宏，生成头文件




