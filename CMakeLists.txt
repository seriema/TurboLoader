cmake_minimum_required(VERSION 3.5)
project(a_retro_ui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(OpenGL REQUIRED)

set(SOURCE_FILES main.cpp)

add_executable(a_retro_ui ${SOURCE_FILES})

target_link_libraries(a_retro_ui
		${SDL2_LIBRARIES}
		${OPENGL_LIBRARIES}
)
