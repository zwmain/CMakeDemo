# 添加生成器表达式

生成器表达式在生成构建系统期间进行求值，以生成特定于每个生成配置的信息。

生成器表达式可以在许多目标属性的上下文中使用，例如`LINK_LIBRARIES`、`INCLUDE_DIRECTORIES`、`COMPILE_DEFINITIONS`等。

当使用命令填充这些属性时，也可以使用它们，例如`target_link_libraries()`、`target_include_directories()`、`target_compile_definition()`等。

生成器表达式可用于启用条件链接、编译时使用的条件定义、条件包含目录等。

条件可以基于构建配置、目标属性、平台信息或任何其他可查询信息。

有不同类型的生成器表达式，包括逻辑表达式、信息表达式和输出表达式。

逻辑表达式用于创建条件输出。基本表达式是0和1表达式。

`$<0:...>`产生空字符串（因为条件是0），`$<1:...>`产生`...`的内容。它们也可以嵌套。

## 练习1：使用INTERFACE库设置C++标准

在使用生成器表达式之前，让我们重构现有代码以使用INTERFACE库。我们将在下一步使用该库来演示生成器表达式的常见用法。

在根CMake中添加一个库，指定为INTERFACE，但是不添加任何源文件。使用`target_compile_features()`指定编译参数。

分别将可执行目标和MathFunctions分别链接到这个没有任何源文件的INTERFACE库，这样就完成了设置C++标准的目的。

## 联系2：使用生成器表达式添加编译器警告标志

生成器表达式的一个常见用法是有条件地添加编译器标志，例如用于语言级别或警告的标志。一种很好的模式是将该信息与允许传播该信息的INTERFACE目标相关联。

生成表达式CMake版本至少为3.15

```cmake
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
```

这里生成表达式为`$<A:B>`，如何解析B依赖于A，结果是0或1。
`COMPILE_LANG_AND_ID`意思是：如果语言符合第一个参数（CXX）且编译器标识符合后面参数，那么结果就是1，否则为0。
`gcc_like_cxx`和`msvc_cxx`最终结果就是1或0。

```cmake
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W3>"
)
```

上面的表达式是逻辑表达式`$<A:B>`，如果A为1，那么结果为B，否则为空字符串。

```cmake
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
```

这个表达式为输出类型表达式，如果是构建项目就启用，否则不启用。




