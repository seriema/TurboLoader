cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(a_retro_ui)

#--- CMAKE SETUP -------------------------------------------------------------------------------------------------------

# Add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/CMake
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

if(APPLE)

	MESSAGE(WARNING "Building for OSX")

	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes -arch i386 -arch x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

	# Platform dependent libraries
	set(PLATFORM_SPECIFIC_LIBRARIES
		"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
		"-framework Foundation" # needed for _NSLog
	)

# Check if we're using PowerShell/Visual Studio or CLion with Cygwin
elseif(MSVC OR CYGWIN)

	MESSAGE(WARNING "Building for Windows (Visual Studio)")

	set(CMAKE_DEBUG_POSTFIX "d")
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation

	# set environmental path for our cmake/Find*.cmake scripts
	set(ENV{SDL2DIR} dependencies/win/SDL2-2.0.4)
	set(ENV{FREEIMAGE_HOME} dependencies/win/FreeImage-3.17.0)

# Check if we're running on Travis OR Raspberry Pi
elseif(UNIX OR EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE(WARNING "Building for Raspberry Pi")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
	)

endif()


#--- PACKAGES SETUP ----------------------------------------------------------------------------------------------------

find_package(SDL2 REQUIRED)
find_package(FreeImage REQUIRED)

# use SDL2
add_definitions(-DENTRY_CONFIG_USE_SDL=1)


#--- CROSS-PLATFORM SETUP ----------------------------------------------------------------------------------------------

set(CROSS_PLATFORM_INCLUDES
	${SDL2_INCLUDE_DIR}
	${FreeImage_INCLUDE_DIRS}
	dependencies/common/glad/include
)

set(CROSS_PLATFORM_LIBRARIES
	${SDL2_LIBRARY}
	${FreeImage_LIBRARIES}
)


#--- EXECUTABLE SETUP --------------------------------------------------------------------------------------------------

include_directories(
	${PLATFORM_SPECIFIC_INCLUDES}
	${CROSS_PLATFORM_INCLUDES}
)

# MAIN
add_executable(a_retro_ui
	main.cpp
	shader.h
	shader.cpp
	shader_program.h
	shader_program.cpp
	platform.h
	Renderer.h
	TextureLoader.h

	dependencies/common/glad/src/glad.c # Separate this into an external library.
)

target_link_libraries(a_retro_ui
	${PLATFORM_SPECIFIC_LIBRARIES}
	${CROSS_PLATFORM_LIBRARIES}
)

MESSAGE("CXX Flags: " ${CMAKE_CXX_FLAGS})
MESSAGE("Platform specific includes: " ${PLATFORM_SPECIFIC_INCLUDES})
MESSAGE("Cross platform includes: " ${CROSS_PLATFORM_INCLUDES})
MESSAGE("Platform specific libraries: " ${PLATFORM_SPECIFIC_LIBRARIES})
MESSAGE("Cross platform libraries: " ${CROSS_PLATFORM_LIBRARIES})
