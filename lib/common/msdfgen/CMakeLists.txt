cmake_minimum_required(VERSION 2.8.4)
project(msdfgen)

# Set variables

if(MSVC OR CYGWIN)
	set(MSDFGEN_LIBRARIES
		lib/freetype
	)
else()
	set(MSDFGEN_LIBRARIES
		freetype
	)
endif()

add_definitions(-DMSDFGEN_STANDALONE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

file(GLOB MSDFGEN_SOURCES
	"core/*.cpp"
	"lib/*.cpp"
	"ext/*.cpp"
)

set(MSDFGEN_INCLUDES
	include
)


# Build

include_directories(
	${MSDFGEN_INCLUDES}
)

add_library(msdfgen
	${MSDFGEN_SOURCES}
)
target_link_libraries(msdfgen
	${MSDFGEN_LIBRARIES}
)

# We currently don't need the executable, only the lib.
#add_executable(msdfgen-executable
#	main.cpp
#)
#target_link_libraries(msdfgen-executable
#	msdfgen
#)
