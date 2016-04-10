cmake_minimum_required(VERSION 2.8)
project(a_retro_ui)

#-------------------------------------------------------------------------------
#add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/CMake
)


#--- BASIC SETUP -------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

#find_library(SDL2_LIBRARY SDL2 REQUIRED) # fixes: undefined reference to `SDL_Init', etc
find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIR})

#find_package(OpenGL REQUIRED)


#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

#check if we're running on Raspberry Pi
if(EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE("Building for Raspberry Pi")

	set(CMAKE_LIBRARY_PATH /opt/vc/lib)

	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
		/opt/vc/lib/libGLESv2.so
#		bcm_host
#		EGL
#		${OPENGLES_LIBRARIES}
	)

elseif(APPLE)

	MESSAGE("Building for OSX")

	# Platform dependent libraries
	set(PLATFORM_SPECIFIC_LIBRARIES
		"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework QuartzCore" # needed for _OBJC_CLASS_$_CAMetalLayer etc
		"-framework Metal" # needed for _MTLCreateSystemDefaultDevice etc Metal stuff
		"-framework Foundation" # needed for _NSLog
	)

elseif(MSVC)

	# JP
#	set(CMAKE_LIBRARY_PATH /cygdrive/c/libs/SDL2-2.0.3/lib)

endif()


#--- EXECUTABLE SETUP -------------------------------------------------------------------------------------------

#INCLUDE(FindPkgConfig)
#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
#INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
#TARGET_LINK_LIBRARIES(sdl2test ${SDL2_LIBRARIES})

find_package(OpenGL REQUIRED)
set(CROSS_PLATFORM_LIBRARIES
	${SDL2_LIBRARY}
#	${SDL2_LIBRARES}
#	${OPENGLES_LIBRARIES}
	${OPENGL_LIBRARIES}
###	SDL2 # fixes: undefined reference to `SDL_Init'
#	OpenGL
)

# MAIN
add_executable(a_retro_ui main.cpp)
target_link_libraries(a_retro_ui
	${PLATFORM_SPECIFIC_LIBRARIES}
	${CROSS_PLATFORM_LIBRARIES}
)
