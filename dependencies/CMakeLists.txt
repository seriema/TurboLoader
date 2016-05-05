# Set up our dependencies
# We use dependency managers when possible, otherwise we add the library here.

cmake_minimum_required(VERSION 2.8.4)
project(dependencies)


#--- CMAKE SETUP -------------------------------------------------------------------------------------------------------

# Add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)


#--- PLATFORM SPECIFIC SETUP -------------------------------------------------------------------------------------------

if(APPLE)
	# nothing right now, but block CMake from triggering the UNIX flag when on OSX.

# Check if we're using PowerShell/Visual Studio or CLion with Cygwin
elseif(MSVC OR CYGWIN)
	# set environmental path for our cmake/Find*.cmake scripts
	set(ENV{SDL2DIR} win/SDL2-2.0.4)
	set(ENV{FREEIMAGE_HOME} win/FreeImage-3.17.0)
	set(ENV{LUA_DIR} win/LuaJIT-2.0.4/src)

# Check if we're running on Travis OR Raspberry Pi
elseif(UNIX OR EXISTS "/opt/vc/include/bcm_host.h")

	set(PLATFORM_SPECIFIC_LIBRARIES
		dl # fixes: undefined reference to symbol 'dlsym@@GLIBC_2.4'
		pthread # fixes: undefined reference to symbol 'sem_post@@GLIBC_2.4'
		rt # fixes: undefined reference to symbol 'clock_gettime@@GLIBC_2.4'
	)

endif()


#--- PACKAGES SETUP ----------------------------------------------------------------------------------------------------

find_package(SDL2 REQUIRED)
find_package(FreeImage REQUIRED)
find_package(Lua51 REQUIRED) #Lua, Lua50, Lua51
add_definitions(-DENTRY_CONFIG_USE_SDL=1) # use SDL2


#--- EXPORT PATHS ------------------------------------------------------------------------------------------------------

set(DEPENDENCIES_INCLUDES
	${CMAKE_CURRENT_SOURCE_DIR}/common/glad/include
	${CMAKE_CURRENT_SOURCE_DIR}/common/Hypodermic
	${SDL2_INCLUDE_DIR}
	${FreeImage_INCLUDE_DIRS}
	${LUA_INCLUDE_DIR}
	${Luaudio_INCLUDE_DIRS}
	PARENT_SCOPE
)

set(DEPENDENCIES_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/common/glad/src/glad.c
	PARENT_SCOPE
)

set(DEPENDENCIES_LIBRARIES
	${PLATFORM_SPECIFIC_LIBRARIES}
	${SDL2_LIBRARY}
	${FreeImage_LIBRARIES}
	${LUA_LIBRARIES}
	PARENT_SCOPE
)

MESSAGE("Lua version: " ${LUA_VERSION_STRING})
