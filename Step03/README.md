# 为库添加使用要求

目标参数的使用要求允许对库或可执行文件的链接和包含行进行更好的控制，同时也可以对CMake中目标的传递属性进行更多的控制。

在本练习中，我们将从Step02中重构代码，以使用现代CMake方法。我们将让我们的库定义自己的使用需求，以便根据需要将它们传递给其他目标。

在这种情况下，MathFunctions将自己指定任何需要的include目录。然后，目标Tutorial只需链接到MathFunctions，而不必担心任何额外的include目录。

我们希望任何使用MathFunctions的目标都需要包含其源文件目录，但是他自己不需要（因为源文件和头文件在同目录）。

这可以用INTERFACE使用需求来表示。

记住：`INTERFACE`意思是使用者需要但是提供者不需要

这样，在顶层CMake中，就直接链接库，而不需要指定include目录

```cmake
add_library(MathFunctions mysqrt.cxx)
target_include_directories(MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

```

```cmake
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

```



