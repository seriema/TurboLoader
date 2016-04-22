# a-retro-ui
A retro UI


**On MacOS**

You need [HomeBrew](http://brew.sh/) and then run:

```bash
$ brew install cmake sdl2
```


Then, generate and build the Makefile with CMake:

```bash
cmake .
make
chmod +x a_retro_ui
```


**On Windows**

*Note: Currently we only test on Windows 8.1 with Visual Studio 2015 (Update 2).*

You need:

* [SDL2](https://www.libsdl.org/download-2.0.php) - tested with [Visual C++ 32/64-bit v2.0.4](https://www.libsdl.org/release/SDL2-devel-2.0.4-VC.zip)

Put SDL2 anywhere and restructure the `lib/` from `lib/x86` and `lib/64` to `lib` and `lib64` so they can be found. Add the SDL2 root path to a system environment variable called `CMAKE_PREFIX_PATH` (`;` separated list). E.g. I put my external libraries in `C:\libs\SDL2-2.0.4` and my `CMAKE_PREFIX_PATH` system environment variable is set to it. The folder looks like:

```
C:\libs\SDL2-2.0.4
\ include
\ lib
\ lib64
```

NOTE: I cheated! I put SDL2.dll in the project root. Checking it in for now.

Then, generate and build the Makefile with CMake:

```powershell
cmake .
make
chmod +x a_retro_ui
```
