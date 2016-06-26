cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(a_retro_ui)

#--- CMAKE SETUP -------------------------------------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "CMakeCache.txt")  # "make clean"

# Add project dependencies before anything else as they need to be able to
# work independently from our project setup.
add_subdirectory("lib")


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

include_directories(
	${DEPENDENCIES_INCLUDES}
)

set(SOURCE_FILES
	${DEPENDENCIES_SOURCES}

	src/platform.h

	src/Ioc_Container.h

	src/Application.h
	src/Application_Builder.h

	src/Environment_Manager.h
	src/Environment_Factory.h
	src/Environment_Factory.cpp
	src/Environment_Manager_SDL_OpenGL.h
	src/Environment_Manager_SDL_OpenGL.cpp

	src/Resource_Aux.h
	src/Resource_Handle.h
	src/Resource_HandleManager.h
	src/Resource_MeshCollection.h
	src/Resource_BitmapCollection.h
	src/Resource_ShaderCollection.h
	src/Resource_PackageCollection.h
	src/Resource_MeshLoader.h
	src/Resource_BitmapLoader.h
	src/Resource_ShaderLoader.h
	src/Resource_ShaderLoader.cpp
	src/Resource_PackageLoader.h
	src/Resource_PackageLoader_Lua.h
	src/Resource_PackageLoader_Lua.cpp

	src/Graphics_RenderKey.h
	src/Graphics_Renderer.h
	src/Graphics_TextureManager.h
	src/Graphics_TextureManager_OpenGL.h
	src/Graphics_ShaderManager.h
	src/Graphics_ShaderManager_OpenGL.h
	src/Graphics_MeshManager.h
	src/Graphics_MeshManager_OpenGL.h

	src/Ecs_Aux.h
	src/Ecs_Entity.h
	src/Ecs_Component.h
	src/Ecs_System.h
	src/Ecs_EntityManager.h
	src/Ecs_Time.h
	src/Ecs_SystemTick.h

	src/Ui_EntityFactory.h
	src/Ui_ComponentRender.h
	src/Ui_ComponentTransform.h
	src/Ui_SystemMain.h
	src/Ui_SystemRenderer.h
	src/Ui_SystemInput.h
	src/Ui_SystemAnimator.h
	src/Ui_ComponentInteraction.h src/Ui_SystemInteraction.h)

# MAIN
add_executable(a_retro_ui
	src/main.cpp
	${SOURCE_FILES}
)
target_link_libraries(a_retro_ui
	${DEPENDENCIES_LIBRARIES}
)

# TEST
add_executable(a_retro_test
	src/test.cpp
	${SOURCE_FILES}
)
target_link_libraries(a_retro_test
	${DEPENDENCIES_LIBRARIES}
)

MESSAGE("CXX Flags: " ${CMAKE_CXX_FLAGS})
MESSAGE("Dependencies includes: " ${DEPENDENCIES_INCLUDES})
MESSAGE("Dependencies libraries: " ${DEPENDENCIES_LIBRARIES})
