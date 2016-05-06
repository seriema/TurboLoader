
#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include <Hypodermic/ContainerBuilder.h>

#include "platform.h"
#include "EnvironmentFactory.h"
#include "EnvironmentManager.h"
#include "Renderer.h"
#include "Application.h"
#include "Application_Main.h"


#define A_RETRO_UI_USE_SDL 1
#define A_RETRO_UI_USE_OPENGL 1


#if defined(A_RETRO_UI_USE_SDL) && defined(A_RETRO_UI_USE_OPENGL)

int main( int argc, char* args[] )
{
	std::cout << "STARTING IN: " << args[ 0 ] << std::endl;
	std::cout << "Initializing..." << std::endl;

	EnvironmentFactory_SDL_OpenGL environment_factory;
	std::shared_ptr< IEnvironmentManager > environment_manager = environment_factory.create_environment_manager();

	if ( environment_manager == nullptr )
		return 1;

	Hypodermic::ContainerBuilder builder;
	builder.registerInstance( environment_manager );
	//builder.registerType< InputManager_SDL >().as< IInputManager >.singleInstance();
	builder.registerType< Renderer_SDL_OpenGL >().as< IRenderer >().singleInstance();
	builder.registerType< Application_Main >().as< IApplication >().singleInstance();

	auto container = builder.build();
	auto app = container->resolve< IApplication >();
	app->loop();

	environment_factory.destroy_environment_manager( environment_manager );

	return 0;
}

#else

int main( int argc, char* args[] )
{
	std::cout << "Wrong environment! Only SDL + OpenGL supported!" << std::endl;
	return 2;
}

#endif

