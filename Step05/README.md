# 安装与测试

## 练习1：安装规则

通常，仅构建一个可执行文件是不够的，它还应该是可安装的。使用CMake，我们可以使用install()命令指定安装规则。

在CMake中支持构建的本地安装通常与指定安装位置以及要安装的目标和文件一样简单。

安装库文件

```cmake
set(installable_libs MathFunctions tutorial_compiler_flags)
install(TARGETS ${installable_libs} DESTINATION lib)

```

安装头文件

```cmake
install(FILES MathFunctions.h DESTINATION include)

```

## 练习2：测试

CTest提供了一种轻松管理项目测试的方法.可以通过`add_test()`命令添加测试。
虽然本教程中没有明确介绍它，但CTest和其他测试框架（如GoogleTest）之间有很多兼容性。

在顶层CMakeLists文件的尾部，使用`enable_testing()`命令，启用测试

启用测试后，我们将添加一些基本测试，以验证应用程序是否正常工作。
首先，我们使用`add_test()`创建一个测试，该测试使用传入的参数25运行Tutorial可执行文件。
对于这个测试，我们不会检查可执行文件的计算结果。
此测试将验证应用程序是否运行，是否存在segfault或其他故障，并且返回值是否为零。这是CTest测试的基本形式。

```cmake
add_test(NAME Runs COMMAND Tutorial 25)

```

这个命令添加一个名为Runs的测试用例，执行Tutorial命令并传入25作为参数。没有结果检查。

接下来，让我们使用`PASS_REGULAR_EXPRESTION`测试属性来验证测试的输出是否包含某些字符串。
在这种情况下，验证在提供的参数数量不正确时是否打印了使用消息。

```cmake
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
)
```

添加一个名为Usage的测试用例，执行Tutorial命令，不带参数。
使用正则表达校验输出结果。正则表达式可以根据需求定义。

```cmake
add_test(NAME StandardUse COMMAND Tutorial 4)
set_tests_properties(StandardUse
  PROPERTIES PASS_REGULAR_EXPRESSION "4 is 2"
  )
```

上面测试用例，检查输出中，是否能匹配`4 is 2`这个字样。

封装函数，以便添加更多测试用例

```cmake
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction()

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is (-nan|nan|0)")
do_test(Tutorial 0.0001 "0.0001 is 0.01")

```

******************************导航到生成目录并重新生成应用程序。
然后，运行ctest命令：`ctest- N`和`ctest -VV`。对于多配置生成器（例如Visual Studio），必须使用`-C ＜mode＞`标志指定配置类型。
例如，要在调试模式下运行测试，请从构建目录（而不是Debug子目录！）中使用`ctest -C Debug -VV`。
Release模式将从同一位置执行，但使用`-C Release`。或者，从IDE构建`RUN_TESTS`目标。




