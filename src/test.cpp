
#include <iostream>

#include "platform.h"
#include "Renderer.h"
#include "Input.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

static int lua_hello_world( lua_State* L )
{
	int argc = lua_gettop( L );
	int argv = lua_tointeger( L, 1 );

	printf( "Got data from Lua :: %d args :: '%d'\n", argc, argv );

	lua_pushnumber( L, argc );
	lua_pushnumber( L, argv );
	return 2; // Number of lua_pushX calls.
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
	L = nullptr;


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
