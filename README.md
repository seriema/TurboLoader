# a-retro-ui
[![Build Status](https://travis-ci.com/seriema/a-retro-ui.svg?token=iZ5rGpFVHQ1ypGqzVJrC&branch=master)](https://travis-ci.com/seriema/a-retro-ui)


## Building the project


### On Raspberry Pi
*Tested on RetroPie +3.7*

Install dependencies:

```bash
$ sudo apt-get install cmake libsdl2-dev libfreeimage-dev lua5.1-dev
```

Then, generate and build the Makefile with CMake:

```bash
cmake .
make
./a_retro_ui
```


### On OSX
*Tested on OSX +10.10*

You need [HomeBrew](http://brew.sh/) and then run:

```bash
$ brew install cmake sdl2 freeimage lua51
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

### Remotely debugging this project

Remotely debugging your raspberry pi from the comfort of your localhost is as easy as running:

## On your raspberry pi

```bash
$ gdbserver 0.0.0.0:6969 ./a_retro_ui
```

## On your localhost

```bash
$ brew install gdb
```

```bash
$ gdb
gdb> target remote [your rpi IP]:6969
```

And that's it! Unfortunately at the time of writing this, fancy editors such as [CLion still lack
the remote debugging feature](https://youtrack.jetbrains.com/issue/CPP-744) .
