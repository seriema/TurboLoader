cmake_minimum_required(VERSION 3.5)
project(a_retro_ui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# include bgfx directories
include_directories(dependencies/bgfx/include)
include_directories(dependencies/bgfx/examples/common)
include_directories(dependencies/bgfx/3rdparty) # needed for remotery/lib/Remotery.h
include_directories(dependencies/bgfx/3rdparty/khronos) # needed for gl/glext.h

# include bx directories
include_directories(dependencies/bx/include)
include_directories(dependencies/bx/include/compat/osx) # needed for malloc.h

# bgfx 3rd party files
set(BGFX_3RDPARTY_FILES
	dependencies/bgfx/3rdparty/ib-compress/indexbufferdecompression.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui_draw.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui_wm.cpp
)

# bgfx example files
file(GLOB BGFX_EXAMPLE_FILES
	dependencies/bgfx/examples/common/*.cpp
	dependencies/bgfx/examples/common/debugdraw/*.cpp
	dependencies/bgfx/examples/common/font/*.cpp
	dependencies/bgfx/examples/common/imgui/*.cpp
	dependencies/bgfx/examples/common/nanovg/*.cpp
	dependencies/bgfx/examples/common/nanovg/*.cpp
	)
list(APPEND BGFX_EXAMPLE_FILES
	dependencies/bgfx/examples/common/entry/cmd.cpp
	dependencies/bgfx/examples/common/entry/dbg.cpp
	dependencies/bgfx/examples/common/entry/input.cpp
	dependencies/bgfx/examples/common/entry/entry.cpp
	dependencies/bgfx/examples/common/entry/entry_osx.mm
)

# Platform dependent libraries
IF(APPLE)
	set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")
	set(PLATFORM_LIBRARIES
		${COCOA_LIBRARY} # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework QuartzCore" # needed for _OBJC_CLASS_$_CAMetalLayer etc
		"-framework Metal" # needed for _MTLCreateSystemDefaultDevice etc Metal stuff
		)
endif(APPLE)

# MAIN
add_executable(a_retro_ui
	main.cpp

	${BGFX_3RDPARTY_FILES}
	${BGFX_EXAMPLE_FILES}
	dependencies/bgfx/src/amalgamated.mm
)
target_link_libraries(a_retro_ui ${PLATFORM_LIBRARIES})
