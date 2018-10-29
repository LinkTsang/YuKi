# YuKi
[WIP] Yet Another Little GUI Library.

## Requirements

### CMake
- Version: 3.10+

### Boost
- Version: 1.68+
- Please set BOOST_ROOT to the root directory containing Boost or BOOST_INCLUDEDIR to the directory containing Boost's headers.

### LLVM
- Version: 7.0.0+
- Please set LLVM_DIR to the root directory containing LLVM.

## Build

### Windows

#### Environment
```cmd
set BOOST_ROOT=path/to/Boost
set LLVM_DIR=path/to/llvm
```

#### Visual Studio 15 2017
```cmd
mkdir build
cd build
cmake -G "Visual Studio 15 2017" ..
cmake --build .
```

#### Visual Studio 15 2017 with Clang
- First, install [LLVM Compiler toolset](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain) for Visual Studio 15 2017.
- Then, build with the LLVM toolset.
```cmd
mkdir build
cd build
cmake -G "Visual Studio 15 2017" -T "llvm" ..
cmake --build .
```
