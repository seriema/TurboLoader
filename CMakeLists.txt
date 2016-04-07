cmake_minimum_required(VERSION 2.8)
project(a_retro_ui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DENTRY_CONFIG_USE_SDL=1)

# include bgfx directories
include_directories(dependencies/bgfx/include)
include_directories(dependencies/bgfx/examples/common)
include_directories(dependencies/bgfx/3rdparty) # needed for remotery/lib/Remotery.h
include_directories(dependencies/bgfx/3rdparty/glsl-optimizer/include) # needed for stdint.h (I think)
include_directories(dependencies/bgfx/3rdparty/glsl-optimizer/src) # needed for stdint.h (I think)
include_directories(dependencies/bgfx/3rdparty/glsl-optimizer/src/mesa) ## needed for stdint.h (I think)
include_directories(dependencies/bgfx/3rdparty/khronos) # needed for gl/glext.h

#ugh...
file(GLOB glsl_sources
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/*.c
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/*.cpp
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/*.c
	dependencies/bgfx/3rdparty/glsl-optimizer/src/mesa/*.c
	dependencies/bgfx/3rdparty/glsl-optimizer/src/mesa/main/*.c
	dependencies/bgfx/3rdparty/glsl-optimizer/src/mesa/program/*.c
	dependencies/bgfx/3rdparty/glsl-optimizer/src/util/*.c
	)
file(GLOB glsl_sources_remove
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/glcpp.cpp
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/ir_set_program_inouts.cpp
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/main.cpp
	dependencies/bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_stubs.cpp
	)
list(REMOVE_ITEM glsl_sources ${glsl_sources_remove})

# bgfx 3rd party files
set(BGFX_3RDPARTY_FILES
	${glsl_sources}
	dependencies/bgfx/3rdparty/ib-compress/indexbufferdecompression.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui_draw.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui_wm.cpp
)
add_library(bgfx_3rdparty
	${BGFX_3RDPARTY_FILES}
)



# include bx directories
include_directories(dependencies/bx/include)

# bgfx files
#check if we're running on Raspberry Pi
MESSAGE("Looking for bcm_host.h (Raspberry PI)")
if(EXISTS "/opt/vc/include/bcm_host.h")
	MESSAGE("bcm_host.h found - assuming this is a Raspberry PI")
	#set(BCMHOST found)
	#set(GLSystem "OpenGL ES")
##	add_definitions(-DBX_CPU_RPI=1)
	add_definitions(-DBGFX_CONFIG_RENDERER_OPENGLES=1)

	add_definitions(-D__STDC_LIMIT_MACROS=1)
	add_definitions(-D__STDC_FORMAT_MACROS=1)
	add_definitions(-D__STDC_CONSTANT_MACROS=1)

	#	add_definitions(-D__STDC_LIMIT_MACROS=1)

#	find_package (Threads) # http://stackoverflow.com/a/29871891

	add_library(bgfx
		dependencies/bgfx/src/amalgamated.cpp
	)

#	include_directories(dependencies/bx/include/compat/freebsd)

	# Platform dependent libraries
	set(PLATFORM_LIBRARIES
#		${COCOA_LIBRARY} # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
	)

else()
	MESSAGE("bcm_host.h not found")
endif()
IF(APPLE)
	MESSAGE("Building for OSX")

	add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=1)

	include_directories(dependencies/bx/include/compat/osx) # needed for malloc.h

	add_library(bgfx
		dependencies/bgfx/src/amalgamated.mm
	)

	# Platform dependent libraries
	set(PLATFORM_LIBRARIES
#		${COCOA_LIBRARY} # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework QuartzCore" # needed for _OBJC_CLASS_$_CAMetalLayer etc
		"-framework Metal" # needed for _MTLCreateSystemDefaultDevice etc Metal stuff
		"-framework Foundation" # needed for _NSLog
		)
endif(APPLE)

find_library(GL_LIBRARY OpenGL REQUIRED)
find_library(SDL2_LIBRARY SDL2 REQUIRED)
set(CROSS_PLATFORM_LIBRARIES
	${SDL2_LIBRARY}
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
