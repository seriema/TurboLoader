#include <iostream>

#include "msdfgen.h"
#include "msdfgen-ext.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include "platform.h"
#include "Renderer.h"
#include "Input.h"

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
	//Smoke test Lua.
	printf( "¿¿ Smoke test Lua ??\n" );
	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	lua_register( L, "lua_hello_world", lua_hello_world );
	luaL_dofile( L, "hello_world.lua" );
	lua_close( L );


	// --- STARTUP ------------------------------

	//Startup renderer.
	printf( "¿¿ Startup renderer ??\n" );
	if ( !init_sdl_gl() )
		return 1;
	Renderer_SDL_OpenGL renderer;
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Unable to initialize OpenGL!\n%d\n", error );
		return false;
	}

	//Startup input.
	printf( "¿¿ Startup input ??\n" );
	Input* input = new Input();


	// --- TEST ---------------------------------

	//Test run input.
	printf( "¿¿ Test run input ??\n" );
	input->poll_events();

	//Test run renderer.
	printf( "¿¿ Test run renderer ??\n" );
	renderer.render();
	SDL_GL_SwapWindow( gWindow );

	//Test run msdf font.
	printf( "¿¿ Test run msdfgen ??\n" );
	msdfgen_hello_world();


	// --- SHUTDOWN -----------------------------

	//Shutdown input.
	printf( "¿¿ Shutdown input ??\n" );
	delete input;

	//Shutdown renderer.
	printf( "¿¿ Shutdown renderer ??\n" );
	shutdown_sdl_gl();

	// --- EXIT ---------------------------------

	//Test successful.
	printf( "¿¿ Test successful ??\n" );
	return 0;
}
