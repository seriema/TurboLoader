
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include "platform.h"
#include "Renderer.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

//Main loop flag
bool quit = false;



static int lua_hello_world( lua_State* L )
{
	int argc = lua_gettop( L );
	int argv = lua_tonumber( L, 1 );

	printf( "Got data from Lua :: %d args :: '%d'\n", argc, argv );

	lua_pushnumber( L, argc );
	lua_pushnumber( L, argv );
	return 2; // Number of lua_pushX calls.
}

void handleKeys( unsigned char key, int x, int y )
{
	if( key == 'q' )
	{
		quit = true;
	}
}

int main( int argc, char* args[] )
{
	std::cout << "STARTING IN: " << args[0] << std::endl;

	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	lua_register( L, "lua_hello_world", lua_hello_world );
	luaL_dofile( L, "hello_world.lua" );
	lua_close( L );
	L = nullptr;

	//Start up SDL and create window
	printf("Initializing...\n");
	if ( !init_sdl_gl() )
		return 1;

	Renderer_SDL_OpenGL renderer;


	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Unable to initialize OpenGL!\n%d\n", error );
		return false;
	}

	printf("----------------------------------------------------------------\n");
	printf("Initialized OpenGL\n");
	printf("OpenGL Info\n");
	printf("    Version: %s\n", glGetString(GL_VERSION));
	printf("     Vendor: %s\n", glGetString(GL_VENDOR));
	printf("   Renderer: %s\n", glGetString(GL_RENDERER));
	printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("----------------------------------------------------------------\n");





	std::vector< std::pair<RenderKey, RenderData> > objects;

	GLfloat vertices[] =
	{
		0.0f,  0.2f, 0.0f,
		-0.15f, 0.0f, 0.0f,
		0.15f, 0.0f, 0.0f,
	};
	int n_vertices = sizeof(vertices) / sizeof(GL_FLOAT);
	const GLuint vbo_handle = renderer.add_mesh( vertices, n_vertices );

	// Allocate triangle 1.
	{
		RenderKey render_key;
		RenderData render_data;

		render_key.RenderOpaque.material_index = 0;
		render_data.vbo_handle = vbo_handle;
		render_data.x = -0.5f;
		render_data.y = -0.1f;

		objects.push_back( { render_key, render_data } );
	}

	// Allocate triangle 2.
	{
		RenderKey render_key;
		RenderData render_data;

		render_key.RenderOpaque.material_index = 0;
		render_data.vbo_handle = vbo_handle;
		render_data.x = 0.3f;
		render_data.y = 0.6f;

		objects.push_back( { render_key, render_data } );
	}

	// Allocate triangle 3.
	{
		RenderKey render_key;
		RenderData render_data;

		render_key.RenderOpaque.material_index = 0;
		render_data.vbo_handle = vbo_handle;
		render_data.x = 0.9f;
		render_data.y = -0.6f;

		objects.push_back( { render_key, render_data } );
	}

	//Event handler
	SDL_Event e;

	//Enable text input
	SDL_StartTextInput();

	printf("Entering main loop\n");
	//While application is running

	uint32_t t0 = SDL_GetTicks() - 17;

	while( !quit )
	{
//		printf("Looping\n");

		uint32_t t1 = SDL_GetTicks();
		uint32_t dt = t1 - t0;
		t0 = t1;

		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
				//Handle keypress with current mouse position
			else if( e.type == SDL_TEXTINPUT )
			{
				int x = 0, y = 0;
				SDL_GetMouseState( &x, &y );
				handleKeys( e.text.text[ 0 ], x, y );
			}
		}

		for ( auto& obj : objects )
		{
			obj.second.x += 0.0001f * dt;
			if ( obj.second.x > 1.1f )
				obj.second.x = -1.1f;
		}

		for ( auto& obj : objects )
		{
			renderer.draw( obj.first, obj.second );
		}

		renderer.render();
		SDL_GL_SwapWindow( gWindow );
	}

	for ( auto obj : objects )
	{
		renderer.del_mesh( obj.second.vbo_handle );
	}

	//Disable text input
	SDL_StopTextInput();


//	printf("ERRORS >>");
//	printf( SDL_GetError() );
//	printf("<< ERRORS");



	//Free resources and close SDL
	shutdown_sdl_gl();

	return 0;
}
