cmake_minimum_required(VERSION 2.8.4)
project(msdfgen)


if(MSVC OR CYGWIN)
	set(ENV{FREETYPE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}) # Makes this unneeded, I hope: 	${CMAKE_CURRENT_SOURCE_DIR}/common/msdfgen/lib/freetype.lib
endif()


find_package(Freetype REQUIRED)


file(GLOB MSDFGEN_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ext/*.cpp
	)

add_library(msdfgen ${MSDFGEN_SOURCES})

target_include_directories(msdfgen PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	)

target_link_libraries(msdfgen
	${FREETYPE_LIBRARIES}
	)
