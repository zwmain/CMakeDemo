# fcitx5-qt编译记录

## 编译环境安装

Fcitx5依赖库

```shell
sudo apt install \
libfcitx5utils-dev \
gettext \
```

CMake模块

```shell
sudo apt install extra-cmake-modules
```

Qt相关依赖库，编译时需要依赖Qt，而Qt依赖于下面这些库

```shell
sudo apt install \
build-essential \
libgl1-mesa-dev \
libwayland-dev \
libvulkan-dev \
libfontconfig1-dev \
libfreetype6-dev \
libx11-dev \
libx11-xcb-dev \
libxext-dev \
libxfixes-dev \
libxi-dev \
libxrender-dev \
libxcb1-dev \
libxcb-cursor-dev \
libxcb-glx0-dev \
libxcb-keysyms1-dev \
libxcb-image0-dev \
libxcb-shm0-dev \
libxcb-icccm4-dev \
libxcb-sync-dev \
libxcb-xfixes0-dev \
libxcb-shape0-dev \
libxcb-randr0-dev \
libxcb-render-util0-dev \
libxcb-util-dev \
libxcb-xinerama0-dev \
libxcb-xkb-dev \
libxkbcommon-dev \
libxkbcommon-x11-dev
```

## 下载Qt

可以去Qt官网使用qt-online-installer安装：[https://www.qt.io/download-qt-installer-oss](https://www.qt.io/download-qt-installer-oss)

也可以去[https://archive.archlinux.org/packages/q/qt5-base/](https://archive.archlinux.org/packages/q/qt5-base/)

或者[https://archive.archlinux.org/packages/q/qt6-base/](https://archive.archlinux.org/packages/q/qt6-base/)

可以根据具体版本下载编译好的Qt版本

## 下载（克隆）Fcitx5-qt源码

注意是**fcitx5**-qt

```shell
git clone https://github.com/fcitx/fcitx5-qt.git
```

一定要区分清楚fcitx和fcitx5

现在fcitx5是最新的输入法框架，之前的太老旧，已经不再用了

## 编译fcitx5-qt

修改CMakeLists.txt

```cmake
option(ENABLE_QT4 "Enable Qt 4" Off)
option(ENABLE_QT5 "Enable Qt 5" Off)
option(ENABLE_QT6 "Enable Qt 6" On)
option(BUILD_ONLY_PLUGIN "Build only plugin" On)
```

根据要编译的Qt版本将后面的Off改成On，不编译的版本，全部改为Off

将`BUILD_ONLY_PLUGIN`改为On，只编译插件

开始编译

在CMakeLists.txt所在文件夹打开终端

```shell
cmake -S . -B build -DCMAKE_PREFIX_PATH="/opt/Qt/6.5.3/gcc_64"
```

`CMAKE_PREFIX_PATH`后面是Qt的路径，这个一定要加，否则找不到Qt

如果执行不成功，就看看上面的依赖库安装了没有；如果都安装了还报错，就根据报错搜索，缺什么就安装什么

执行成功后再执行

```shell
cmake --build build --config Release
```

等待执行完毕，再执行

```shell
cmake --install build
```

就会发现生成的`libfcitx5platforminputcontextplugin.so`会被安装（其实就是复制）到`/opt/Qt/6.5.3/gcc_64/plugins/platforminputcontexts`文件夹下

这个文件夹也正好是上面指定的Qt路径下的plugins文件夹，相当于给Qt打了一个“补丁”

## QtCreator

但是到这里还不够

`libfcitx5platforminputcontextplugin.so`库在`build/qt6/platforminputcontext/libfcitx5platforminputcontextplugin.so`目录下

我们需要手动将其复制到不能正常输入的软件里去

以QtCreator为例

其目录所在为`/opt/Qt/Tools/QtCreator`

而且QtCreator并不会调用系统的Qt插件

即：

```txt
/usr/lib/x86_64-linux-gnu/qt5/plugins/platforminputcontexts/
```

和

```txt
/usr/lib/x86_64-linux-gnu/qt6/plugins/platforminputcontexts/
```

QtCreator链接的是自己的Qt库，所以插件也要安装到其自己的plugins目录下

```txt
/opt/Qt/Tools/QtCreator/lib/Qt/plugins/platforminputcontexts
```

将`libfcitx5platforminputcontextplugin.so`复制到目标目录下，并且使用`chmode +x`增加执行权限

然后就可以在QtCreator里面输入中文了

## 其他软件

因为这个QtCreator是和Qt一起安装的，因此版本也是Qt6

上面用Qt6编译的插件就可以起作用

对于其他软件

要找到其安装目录下的可执行文件或者`QtCore.so`之类的文件

使用`strings QtCore.so | grep Qt`查看Qt版本

知道了Qt版本后，流程和上面一样，最后把插件复制到软件安装目录的`plugins/platforminputcontexts`，没有就新建

## 错误及解决方法

```txt
CMake Error at CMakeLists.txt:9 (find_package):
  By not providing "FindECM.cmake" in CMAKE_MODULE_PATH this project has
  asked CMake to find a package configuration file provided by "ECM", but
  CMake did not find one.

  Could not find a package configuration file provided by "ECM" (requested
  version 1.4.0) with any of the following names:

    ECMConfig.cmake
    ecm-config.cmake

  Add the installation prefix of "ECM" to CMAKE_PREFIX_PATH or set "ECM_DIR"
  to a directory containing one of the above files.  If "ECM" provides a
  separate development package or SDK, be sure it has been installed.

```

解决方法：

```shell
sudo apt install extra-cmake-modules
```

```txt
CMake Error at CMakeLists.txt:52 (find_package):
  By not providing "FindFcitx5Utils.cmake" in CMAKE_MODULE_PATH this project
  has asked CMake to find a package configuration file provided by
  "Fcitx5Utils", but CMake did not find one.

  Could not find a package configuration file provided by "Fcitx5Utils"
  (requested version 5.0.16) with any of the following names:

    Fcitx5UtilsConfig.cmake
    fcitx5utils-config.cmake

  Add the installation prefix of "Fcitx5Utils" to CMAKE_PREFIX_PATH or set
  "Fcitx5Utils_DIR" to a directory containing one of the above files.  If
  "Fcitx5Utils" provides a separate development package or SDK, be sure it
  has been installed.

```

解决方法：


```shell
sudo apt install libfcitx5utils-dev
```


```txt
CMake Error at /usr/share/cmake-3.25/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find Gettext (missing: GETTEXT_MSGMERGE_EXECUTABLE
  GETTEXT_MSGFMT_EXECUTABLE)
Call Stack (most recent call first):
  /usr/share/cmake-3.25/Modules/FindPackageHandleStandardArgs.cmake:600 (_FPHSA_FAILURE_MESSAGE)
  /usr/share/cmake-3.25/Modules/FindGettext.cmake:81 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  CMakeLists.txt:69 (find_package)

```

解决方法：


```shell
sudo apt install gettext
```

```txt
CMake Error at qt6/CMakeLists.txt:2 (find_package):
  Could not find a package configuration file provided by "Qt6" (requested
  version 6.0.0) with any of the following names:

    Qt6Config.cmake
    qt6-config.cmake

  Add the installation prefix of "Qt6" to CMAKE_PREFIX_PATH or set "Qt6_DIR"
  to a directory containing one of the above files.  If "Qt6" provides a
  separate development package or SDK, be sure it has been installed.
```

解决方法：


```shell
cmake -S . -B build -DCMAKE_PREFIX_PATH="/opt/Qt/6.5.2/gcc_64/"
```

```txt
CMake Error at qt6/CMakeLists.txt:5 (find_package):
  Found package configuration file:

    /opt/Qt/6.5.2/gcc_64/lib/cmake/Qt6WaylandClient/Qt6WaylandClientConfig.cmake

  but it set Qt6WaylandClient_FOUND to FALSE so package "Qt6WaylandClient" is
  considered to be NOT FOUND.  Reason given by package:

  Qt6WaylandClient could not be found because dependency Wayland could not be
  found.

  Configuring with --debug-find-pkg=Wayland might reveal details why the
  package was not found.

  Configuring with -DQT_DEBUG_FIND_PACKAGE=ON will print the values of some
  of the path variables that find_package uses to try and find the package.

```

解决方法：


```shell
sudo apt install \
build-essential \
libgl1-mesa-dev \
libwayland-dev \
libvulkan-dev
```


