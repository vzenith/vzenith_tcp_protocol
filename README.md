[![Build Status](https://travis-ci.org/vzenith/vzenith_tcp_protocol.svg?branch=master)](https://travis-ci.org/vzenith/vzenith_tcp_protocol)
[![Coverage Status](https://coveralls.io/repos/github/vzenith/vzenith_tcp_protocol/badge.svg?branch=master)](https://coveralls.io/github/vzenith/vzenith_tcp_protocol?branch=master)

# vzenith_tcp_protocol

## 当前状态

- [travis-ci.org](https://travis-ci.org/vzenith/vzenith_tcp_protocol "travis-ci.org") 已经编译通过，同时也在Windows平台上面编译通过。
- [coveralls.io](https://coveralls.io/github/vzenith/vzenith_tcp_protocol?branch=master")，单元测试覆盖率达到86%
- 在Ubuntu 64平台下，使用valgrind对所有的单元测试程序进行内存检查通过。您可以使用 `git_path/vzenith_tcp_protocol/valgrind_test/valgrind_test.sh`这个脚本文件对所有的单元测试进行验证。
- [linthub.io](https://linthub.io/vzenith/vzenith_tcp_protocol/283b48c98896c1577fb21f60ee675ab169b2317e) 代码风格检查完成。linthub对于C++的代码风格检查使用的是`cpplint`，但是却不支持一些过滤器，比如禁止对C++11代码风格的警告，因此有一些代码风格被注释了，并没有完全遵守linthub.io的检查结果。不过可以在本地进行下载 [cpplint.py](https://pypi.python.org/pypi/cpplint") 进行代码风格检查，同时使用如下的过滤参数 `--filter=-build/include_what_you_use,-runtime/references,-readability/casting,-build/c++11,-runtime/printf,-runtime/threadsafe_fn`对`git_path/vzenith_tcp_protocol/src`目录下的代码进行风格检查。

## 编译配置

环境要求

- `CMake2.8.5 `,工程使用CMake来管理源代码，因此你必须要确保在您的计算机上面有`CMake 2.8.5`以上的版本。

当前的项目已经在以下几个平台上编译通过

- Windows Microsoft Visual Studio 2013 
- Ubuntu 64 GCC 4.8
- LINUX Clang++ 3

### 下载工程

```bash
	// 下载源代码工程
	git clone https://github.com/vzenith/vzenith_tcp_protocol.git
	// 进入下载工程目录	
	cd vzenith_tcp_protocol
	// 更新第三方依赖库
	git submodule update --init --recursive
	// 创建一个编译文件夹，在这个文件夹中进行编译，保证源代码清洁
	mkdir build
	// 各个平台的编译方式参考下面的教程
	cd build
	// 执行CMake生成相应的工程文件
```

我们将下载源代码的路径称为`git_path`路径。通过上面的几个步骤您现在应该处于`git_path/vzenith_tcp_protocol/build`下。

请确保您已经安装了CMake2.8.5以上的版本，并且已经加入到bash的path路径中，然后执行CMake生成工程

```bash
cmake .. -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DENABLE_UNIT_TEST=OFF
```

这一条命令会在Windows平台上面生成VS工程，在Linux平台上面生成Makefile文件。

### Windows平台上编译并使用

在Windows平台上生成的是VS的工程，可以直接打开进行编译。特别注意我们的编译是有顺序的，一般情况下先编译库，再编译相关的测试程序。虽然我们已经在CMake中指定了编译顺序，但是由于VS一般都采用多线程编译技术，因此一些代码量比较少的测试工程有可能会先编译起，在这种情况下，一些依赖库还没有编译好，所以会出现找不到库的问题。再使用VS编译一次就行了。

编译好了之后，生成的所有静态库，包括第三方静态库都会被放置到`git_path/vzenith_tcp_protocol/lib/win`下面。正常情况下，你在这个目录下应该会看到以下几个库

- `jsoncpp.lib`，由JsonCpp生成的解析Json库
- `libvztcpsdk.lib`，当前所编译的Vzenith TCP Protocol C++ SDK库

**Windows用户使用注意**

1. 默认情况下，在Windows平台上都是使用的MT/MtD的形式编译的，如果想要切换请在VS工程上修改一下就可以了。

### Linux平台上编译安装

Linux平台我们已经指定了编译顺序，只需要使用`make`命令执行，就可以把整个工程全部编译起。正常情况下会生成以下几个文件

- `git_path/vzenith_tcp_protocol/lib/unix`下是所有的库文件，主要包含
	- `libvztcpsdk.a`，主要的`libvztcpsdk`工程库
	- `libjsoncpp.a`，由jsoncpp生成的静态库
- `git_path/vzenith_tcp_protocol/bin/unix`，是所有的测试工程，只包含关于`libvztcpsdk`的测试工程。

**Linux用户使用注意**