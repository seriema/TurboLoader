# a-retro-ui
[![Build Status](https://travis-ci.com/seriema/a-retro-ui.svg?token=iZ5rGpFVHQ1ypGqzVJrC&branch=add-travis-ci)](https://travis-ci.com/seriema/a-retro-ui)


## Building the project


### On Raspberry Pi
*Tested on RetroPie +3.7*

```bash
cmake .
make
./a_retro_ui
```


### On OSX
*Tested on OSX +10.10*

You need [HomeBrew](http://brew.sh/) and then run:

```bash
$ brew install cmake sdl2
```

Then, generate and build the Makefile with CMake:

```bash
cmake .
make
./a_retro_ui
```


### On Windows
*Tested on Windows 8.1 with Visual Studio 2015 (Update 2)*

You need to have Visual Studio, and then install [CMake](https://cmake.org/download/). We recommend [CMake 3.5.0](https://cmake.org/files/v3.5/cmake-3.5.2-win32-x86.msi).

Then, generate and build the Visual Studio Solution with CMake:

```powershell
cmake .
msbuild .\ALL_BUILD.vcxproj
.\Debug\a_retro_ui.exe
```
