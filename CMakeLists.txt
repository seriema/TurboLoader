cmake_minimum_required(VERSION 2.8)
project(a_retro_ui)

#--- BASIC SETUP -------------------------------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

set(CMAKE_LIBRARY_PATH /opt/vc/lib)

#find_library(SDL2_LIBRARY SDL2 REQUIRED) # fixes: undefined reference to `SDL_Init', etc
#find_package(SDL2 REQUIRED)


#--- SDL2 SETUP --------------------------------------------------------------------------------------------------------

# use SDL2
add_definitions(-DENTRY_CONFIG_USE_SDL=1)

#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

#check if we're running on Raspberry Pi
if(EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE(WARNING Building for Raspberry Pi)

	find_package(OpenGL REQUIRED)

	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
		/opt/vc/lib/libGLESv2.so
#		bcm_host
#		EGL
		${OPENGLES_LIBRARIES}
	)

else(APPLE)

	MESSAGE(WARNING "Building for OSX")

	find_package(OpenGL REQUIRED)

	# Platform dependent libraries
	set(PLATFORM_SPECIFIC_LIBRARIES
		"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
#		"-framework QuartzCore" # needed for _OBJC_CLASS_$_CAMetalLayer etc
#		"-framework Metal" # needed for _MTLCreateSystemDefaultDevice etc Metal stuff
		"-framework Foundation" # needed for _NSLog
		${OPENGL_LIBRARIES}
	)

endif()


#--- EXECUTABLE SETUP --------------------------------------------------------------------------------------------------

#INCLUDE(FindPkgConfig)
#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
#INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
#TARGET_LINK_LIBRARIES(sdl2test ${SDL2_LIBRARIES})

set(CROSS_PLATFORM_LIBRARIES
#	${SDL2_LIBRARY}
#	${SDL2_LIBRARES}
#	${OPENGLES_LIBRARIES}
	SDL2 # fixes: undefined reference to `SDL_Init'
#	OpenGL
)

# MAIN
add_executable(a_retro_ui
		main.cpp
		shader.h
		shader.cpp
		shader_program.h
		shader_program.cpp)

target_link_libraries(a_retro_ui
	${PLATFORM_SPECIFIC_LIBRARIES}
	${CROSS_PLATFORM_LIBRARIES}
)
