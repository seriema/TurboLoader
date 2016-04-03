cmake_minimum_required(VERSION 3.5)
project(a_retro_ui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DENTRY_CONFIG_USE_SDL=1)
add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=1)

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
add_library(bgfx_3rdparty
	${BGFX_3RDPARTY_FILES}
)

# bgfx files
add_library(bgfx
	dependencies/bgfx/src/amalgamated.mm
)

# Platform dependent libraries
IF(APPLE)
	set(PLATFORM_LIBRARIES
		${COCOA_LIBRARY} # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
	)
endif(APPLE)

#find_library(GL_LIBRARY OpenGL REQUIRED)
set(CROSS_PLATFORM_LIBRARIES
	${SDL2_LIBRARIES}
#	${GL_LIBRARY}
)

# MAIN
add_executable(a_retro_ui main.cpp)
target_link_libraries(a_retro_ui
	#bgfx_3rdparty
	bgfx
	${PLATFORM_LIBRARIES}
	${CROSS_PLATFORM_LIBRARIES}
)
