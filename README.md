# IMSEE-SDK

## MiyakoMeow补充：Windows下安装记录

### Vcpkg+CMake
```bat
git clone https://github.com/Microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install pcl[tools,visualization]:x64-windows
```
（最后一行估计要爬墙+等编译等一个小时）<br>
假设vcpkg.exe所在目录VCPKG_DIR为`D:\Libraries\vcpkg`<br>
那就将CMAKE_TOOLCHAIN_FILE环境变量设为`D:\Libraries\vcpkg\scripts\buildsystems\vcpkg.cmake`<br>

### OpenCV
> 我把lib目录里的默认dll改成了对应OpenCV3.4.3的版本<br>
> 还是第一次见把代码和某个OpenCV版本锁死的，绝活<br>

下载OpenCV3.4.3（必须得是这个版本）<br>
文件名：opencv-3.4.3-vc14_vc15.exe<br>
打开，解压，把它放到想要的位置<br>
将OpenCV_DIR环境变量设为`D:\Libraries\OpenCV343\build\x64\vc15\lib`<br>
上面这个取决于你把OpenCV放在哪个目录<br>
Path中添加一行`%OpenCV_DIR%\..\bin`<br>

### 编译demo之最后步骤
一定要开启Git的autocrlf，设置为true！！！（在Windows下是默认设置，所以如果你没动过这个设置就不用管了）<br>
编译demo时使用build_demo.bat，CMake直接Configure，Generate，关闭一气呵成<br>
VS里会有一堆警告，不知道会对实际运行有什么影响，编译能通过就行<br>


## 概述

IMSEE-SDK是indemind双目惯性模组的软件开发工具包。模组采用“双目摄像头+IMU”多传感器融合架构与微秒级时间同步机制，为视觉SLAM研究提供精准稳定数据源。模组运用摄像头+IMU多传感器融合架构，使摄像头与IMU传感器优势互补，实现位姿精度更高、环境适应性更强、动态性能更稳定、成本更低的双目立体视觉硬件方案 。

以下平台已经经过测试:

* Windows 10
* Ubuntu 16.04 / 14.04
* Jetson TX2
* RK3399 / 3328

详细信息请参照以下文档

## 文档

* [API Doc](https://github.com/indemind/IMSEE-SDK/releases): API reference, some guides and data spec.
  * zh-Hans: [![](https://img.shields.io/badge/Online-HTML-blue.svg?style=flat)](https://imsee-sdk-docs.readthedocs.io/zh/latest/)



## License

This project is licensed under the [Apache License, Version 2.0](LICENSE). Copyright 2020 Indemind Co., Ltd.

