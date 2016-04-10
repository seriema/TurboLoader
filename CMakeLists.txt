cmake_minimum_required(VERSION 2.8.4)
project(a_retro_ui)

#--- CMAKE SETUP -------------------------------------------------------------------------------------------------------

# Add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/CMake
)


#--- PACKAGES SETUP ----------------------------------------------------------------------------------------------------

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

#--- SDL2 SETUP --------------------------------------------------------------------------------------------------------

# use SDL2
add_definitions(-DENTRY_CONFIG_USE_SDL=1)

#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------
#---  COMPILER SETUP ---------------------------------------------------------------------------------------------------

if(MSVC)
	set(CMAKE_DEBUG_POSTFIX "d")
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation
elseif(CMAKE_COMPILER_IS_GNUCXX)
	#set up compiler flags for GCC
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes -O3") #support C++11 for std::, optimize
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3")  #-s = strip binary
else() #??
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()



#--- PLATFORM LIBRARIES SETUP ------------------------------------------------------------------------------------------

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

elseif(APPLE)

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

elseif(MSVC)

	# JP
	#set(CMAKE_LIBRARY_PATH c:/libs/SDL2-2.0.4/lib64)

endif()


#--- EXECUTABLE SETUP --------------------------------------------------------------------------------------------------
#--- CROSS-PLATFORM LIBRARIES SETUP ------------------------------------------------------------------------------------

include_directories(${SDL2_INCLUDE_DIR})

set(CROSS_PLATFORM_LIBRARIES
#	${SDL2_LIBRARY}
#	${SDL2_LIBRARES}
#	${OPENGLES_LIBRARIES}
	SDL2 # fixes: undefined reference to `SDL_Init'
#	OpenGL
)


#--- EXECUTABLE SETUP --------------------------------------------------------------------------------------------------

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
