
#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include "platform.h"
#include "EnvironmentFactory.h"
#include "EnvironmentManager.h"
#include "Renderer.h"
#include "Application.h"
#include "Application_Main.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define A_RETRO_UI_USE_SDL 1
#define A_RETRO_UI_USE_OPENGL 1


#if defined(A_RETRO_UI_USE_SDL) && defined(A_RETRO_UI_USE_OPENGL)

static int lua_hello_world( lua_State* L )
{
	int argc = lua_gettop( L );
	int argv = lua_tointeger( L, 1 );

	printf( "Got data from Lua :: %d args :: '%d'\n", argc, argv );

	lua_pushnumber( L, argc );
	lua_pushnumber( L, argv );
	return 2; // Number of lua_pushX calls.
}

static void msdfgen_hello_world()
{
	msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
	if ( ft )
	{
		msdfgen::FontHandle* font = loadFont( ft, "OpenSans.ttf" );
		if ( font )
		{
			msdfgen::Shape shape;
			if ( loadGlyph( shape, font, 'A' ))
			{
				shape.normalize();
				// max. angle
				edgeColoringSimple( shape, 3.0 );
				// image width, height
				msdfgen::Bitmap<msdfgen::FloatRGB> msdf( 32, 32 );
				// range, scale, translation
				generateMSDF( msdf, shape, 4.0, 1.0, msdfgen::Vector2( 4.0, 4.0 ));
			}
			destroyFont( font );
		}
		deinitializeFreetype( ft );
	}
}

int main( int argc, char* args[] )
{
	std::cout << "STARTING IN: " << args[ 0 ] << std::endl;
	std::cout << "Initializing..." << std::endl;


	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	lua_register( L, "lua_hello_world", lua_hello_world );
	luaL_dofile( L, "hello_world.lua" );
	lua_close( L );
	L = nullptr;

	// Just make sure it runs.
	msdfgen_hello_world();

	EnvironmentFactory_SDL_OpenGL environment_factory;
	std::shared_ptr< IEnvironmentManager > environment_manager = environment_factory.create_environment_manager();

	if ( environment_manager == nullptr )
		return 1;

	//IInputManager* input_manager = new InputManager_SDL();
	std::shared_ptr< IRenderer > renderer = std::make_shared< Renderer_SDL_OpenGL >();
	std::shared_ptr< IApplication > app = std::make_shared< Application_Main >( environment_manager, renderer );

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
