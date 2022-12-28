# 添加自定义命令和生成的文件

假设，出于本教程的目的，我们决定不使用平台`log`和`exp`函数，而是希望生成一个用于`mysqrt`函数的预计算值表。
在本节中，我们将在构建过程中创建表，然后将该表编译到应用程序中。

移除上一节添加到CMakeLists和源文件中的代码。

文件中已经给了`MakeTable.cxx`文件

查看文件后，我们可以看到表是作为有效的C++代码生成的，并且输出文件名是作为参数传入的。

下一步是将适当的命令添加到`MathFunctions/CMakeLists.txt`文件以构建MakeTable可执行文件，然后将其作为构建过程的一部分运行。要实现这一点，需要几个命令。

首先，在`MathFunctions/CMakeLists.txt`的顶部，添加MakeTable的可执行文件，就像添加任何其他可执行文件一样。

```cmake
add_executable(MakeTable MakeTable.cxx)

```

然后我们添加一个自定义命令，指定如何通过运行MakeTable来生成`Table.h`。

```cmake
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )

```

接下来，我们必须让CMake知道`mysqrt.cxx`依赖于生成的文件`Table.h`。这是通过将生成的`Table.h`添加到库MathFunctions的源列表来完成的。

```cmake
add_library(MathFunctions
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
)

```

我们还必须将当前二进制目录添加到包含目录列表中，以便`mysqrt.cxx`可以找到并包含`Table.h`。

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE   ${CMAKE_CURRENT_BINARY_DIR}
)

```

使用`Table.h`重写`mysqrt`

```c++
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

```

构建名编译项目，会自动生成`Table.h`，并执行`MakeTable`命令。




