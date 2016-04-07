cmake_minimum_required(VERSION 2.8)
project(a_retro_ui)

#--- BASIC SETUP -------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

set(CMAKE_LIBRARY_PATH /opt/vc/lib)

#find_library(SDL2_LIBRARY SDL2 REQUIRED) # fixes: undefined reference to `SDL_Init', etc
#find_package(SDL2 REQUIRED)


#--- BGFX SETUP --------------------------------------------------------------------------------------------------------

# use SDL2
add_definitions(-DENTRY_CONFIG_USE_SDL=1)
# set debug output
add_definitions(-DBGFX_CONFIG_DEBUG=1)


# include bgfx directories
include_directories(dependencies/bgfx/include)
include_directories(dependencies/bgfx/examples/common)
include_directories(dependencies/bgfx/3rdparty) # needed for remotery/lib/Remotery.h
include_directories(dependencies/bgfx/3rdparty/khronos) # needed for gl/glext.h

# bgfx 3rd party files
set(BGFX_3RDPARTY_FILES
	dependencies/bgfx/3rdparty/ib-compress/indexbufferdecompression.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui_draw.cpp
	dependencies/bgfx/3rdparty/ocornut-imgui/imgui_wm.cpp
)



#--- BX SETUP ----------------------------------------------------------------------------------------------------------

# include bx directories
include_directories(dependencies/bx/include)


#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

#check if we're running on Raspberry Pi
if(EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE("Building for Raspberry Pi")

	add_definitions(-DBGFX_CONFIG_RENDERER_OPENGLES=1)
# turning either of these on gives "Segmentation fault"
#	add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=1)
#	add_definitions(-DBGFX_CONFIG_MULTITHREADED=0)

	# Fixes "UINT8_MAX was not declared in this scope" type errors (stdint.h fail).
	# See https://github.com/bkaradzic/bgfx/issues/248#issuecomment-122395431
	add_definitions(-D__STDC_LIMIT_MACROS=1)
	add_definitions(-D__STDC_FORMAT_MACROS=1)
	add_definitions(-D__STDC_CONSTANT_MACROS=1)
# these give "warning: "BX_PLATFORM_RPI" redefined [enabled by default]"
#	add_definitions(-DBGFX_USE_EGL=1)
#	add_definitions(-DBX_PLATFORM_RPI=1)

	set(BGFX_AMALGAMATED_SOURCE
		dependencies/bgfx/src/amalgamated.cpp
	)

	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
		/opt/vc/lib/libGLESv2.so
#		bcm_host
#		EGL
#		${OPENGLES_LIBRARIES}
	)

else(APPLE)

	MESSAGE("Building for OSX")

	add_definitions(-DBGFX_CONFIG_RENDERER_OPENGL=1)

	include_directories(dependencies/bx/include/compat/osx) # needed for malloc.h

	set(BGFX_AMALGAMATED_SOURCE
		dependencies/bgfx/src/amalgamated.mm
	)

	# Platform dependent libraries
	set(PLATFORM_SPECIFIC_LIBRARIES
		"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework QuartzCore" # needed for _OBJC_CLASS_$_CAMetalLayer etc
		"-framework Metal" # needed for _MTLCreateSystemDefaultDevice etc Metal stuff
		"-framework Foundation" # needed for _NSLog
	)

endif()


#--- EXECUTABLE SETUP -------------------------------------------------------------------------------------------

#INCLUDE(FindPkgConfig)
#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
#INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
#TARGET_LINK_LIBRARIES(sdl2test ${SDL2_LIBRARIES})

find_package(OpenGL REQUIRED)
set(CROSS_PLATFORM_LIBRARIES
#	${SDL2_LIBRARY}
#	${SDL2_LIBRARES}
#	${OPENGLES_LIBRARIES}
	${OPENGL_LIBRARIES}
	SDL2 # fixes: undefined reference to `SDL_Init'
#	OpenGL
)

add_library(bgfx_3rdparty ${BGFX_3RDPARTY_FILES}) # Doesn't seem to be needed?
add_library(bgfx ${BGFX_AMALGAMATED_SOURCE})

# MAIN
add_executable(a_retro_ui main.cpp)
target_link_libraries(a_retro_ui
	bgfx
	bgfx_3rdparty
	${PLATFORM_SPECIFIC_LIBRARIES}
	${CROSS_PLATFORM_LIBRARIES}
)
