# Step12.打包Debug和Release

注意：此示例适用于单配置生成器，不适用于多配置生成器（例如Visual Studio）。

默认情况下，CMake的模型是构建目录只包含一个配置，无论是Debug、Release、MinSizeRel还是RelWithDebInfo。但是，可以将CPack设置为捆绑多个构建目录，并构建包含同一项目的多个配置的包。



