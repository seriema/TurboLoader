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

if(APPLE)

	MESSAGE(WARNING "Building for OSX")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

	# Platform dependent libraries
	set(PLATFORM_SPECIFIC_LIBRARIES
	"-framework Cocoa" # needed for _OBJC_CLASS_$_NSOpenGLPixelFormat etc
	"-framework Foundation" # needed for _NSLog
	)

#check if we're running on Travis OR Raspberry Pi
elseif(UNIX OR EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE(WARNING "Building for Raspberry Pi")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
	)

elseif(MSVC)

	MESSAGE(WARNING "Building for Windows (Visual Studio)")

	set(CMAKE_DEBUG_POSTFIX "d")
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation

endif()


#--- CROSS-PLATFORM SETUP ----------------------------------------------------------------------------------------------

set(CROSS_PLATFORM_INCLUDES
	${SDL2_INCLUDE_DIR}
	dependencies/Glad/include
)

set(CROSS_PLATFORM_LIBRARIES
	${SDL2_LIBRARY}
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

	dependencies/Glad/src/glad.c # Separate this into an external library.
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
