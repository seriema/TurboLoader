cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(a_retro_ui)

#--- CMAKE SETUP -------------------------------------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "CMakeCache.txt")  # "make clean"


#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

if(APPLE)

	MESSAGE(WARNING "Building for OSX")

	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes -arch i386 -arch x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

# Check if we're using PowerShell/Visual Studio or CLion with Cygwin
elseif(MSVC OR CYGWIN)

	MESSAGE(WARNING "Building for Windows (Visual Studio)")

	set(CMAKE_DEBUG_POSTFIX "d")
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation

# Check if we're running on Travis OR Raspberry Pi
elseif(UNIX OR EXISTS "/opt/vc/include/bcm_host.h")

	MESSAGE(WARNING "Building for Raspberry Pi")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-attributes")

endif()


#--- MAIN EXECUTABLE SETUP ---------------------------------------------------------------------------------------------

# Add project dependencies
add_subdirectory("lib")

include_directories(
	${DEPENDENCIES_INCLUDES}
)

# MAIN
add_executable(a_retro_ui
#	src/main.cpp
#	src/shader.h
#	src/shader.cpp
#	src/shader_program.h
#	src/shader_program.cpp
#	src/platform.h
#	src/Renderer.h
#	src/TextureLoader.h
#	src/Input.cpp
#	src/Input.h
	msdfgen-test.cpp

	${DEPENDENCIES_SOURCES}
)

target_link_libraries(a_retro_ui
	${DEPENDENCIES_LIBRARIES}
)

MESSAGE("CXX Flags: " ${CMAKE_CXX_FLAGS})
MESSAGE("Dependencies includes: " ${DEPENDENCIES_INCLUDES})
MESSAGE("Dependencies libraries: " ${DEPENDENCIES_LIBRARIES})
