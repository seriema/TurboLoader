cmake_minimum_required(VERSION 2.8.4)
project(a_retro_ui)

#--- CMAKE SETUP -------------------------------------------------------------------------------------------------------

# Add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/CMake
)


#--- PACKAGES SETUP ----------------------------------------------------------------------------------------------------

find_package(SDL2 REQUIRED)


#--- SDL2 SETUP --------------------------------------------------------------------------------------------------------

# use SDL2
add_definitions(-DENTRY_CONFIG_USE_SDL=1)


#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

#check if we're running on Raspberry Pi
if(EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE(WARNING "Building for Raspberry Pi")

	find_package(OpenGLES REQUIRED)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

	if ( OPENGLES_FOUND )
		MESSAGE( WARNING "FOUND OPENGLES" )
	else()
		MESSAGE( WARNING "COULDNT FIND OPENGLES" )
	endif()

	MESSAGE("OpenGL ES libraries: " ${OPENGLES_LIBRARIES})
	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
		${OPENGLES_LIBRARIES}
	)

	include_directories(dependencies) # needed for rpi

elseif(APPLE)

	MESSAGE(WARNING "Building for OSX")

	# SDL2 breaks because both gl.h and gl3.h are included, should be fixed in v2.0.4
	# http://stackoverflow.com/questions/19072278/sdl2-mac-os-x-and-opengl-how-to-avoid-including-both-gl-h-and-gl3-h
	add_definitions(-D__gl_h_=1)
	add_definitions(-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED=1)

	find_package(OpenGL REQUIRED)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

	# Platform dependent libraries
	set(PLATFORM_SPECIFIC_LIBRARIES
		"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework Foundation" # needed for _NSLog
		${OPENGL_LIBRARIES}
	)

elseif(MSVC)

	MESSAGE(WARNING "Building for Windows (Visual Studio)")

	find_package(OpenGL REQUIRED)

	set(CMAKE_DEBUG_POSTFIX "d")
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation

	include_directories(
#		${OPENGL_INCLUDE_DIR}
	)

	include_directories(dependencies)

	set(PLATFORM_SPECIFIC_LIBRARIES
		${OPENGL_LIBRARIES}
	)


endif()


#--- CROSS-PLATFORM LIBRARIES SETUP ------------------------------------------------------------------------------------

include_directories(
	${SDL2_INCLUDE_DIR}
)
MESSAGE("Cross platform includes: " ${SDL2_INCLUDE_DIR})

set(CROSS_PLATFORM_LIBRARIES
	#SDL2 # fixes: undefined reference to `SDL_Init'
	${SDL2_LIBRARY}
)


#--- EXECUTABLE SETUP --------------------------------------------------------------------------------------------------

# MAIN
add_executable(a_retro_ui
	main.cpp
	shader.h
	shader.cpp
	shader_program.h
	shader_program.cpp
	platform.h
)

target_link_libraries(a_retro_ui
	${PLATFORM_SPECIFIC_LIBRARIES}
	${CROSS_PLATFORM_LIBRARIES}
)

MESSAGE("CXX Flags: " ${CMAKE_CXX_FLAGS})
MESSAGE("Platform specific libraries: " ${PLATFORM_SPECIFIC_LIBRARIES})
MESSAGE("Cross platform libraries: " ${CROSS_PLATFORM_LIBRARIES})
