#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

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
	std::cout << "STARTING IN: " << args[0] << std::endl;

	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	lua_register( L, "lua_hello_world", lua_hello_world );
	luaL_dofile( L, "hello_world.lua" );
	lua_close( L );
	L = nullptr;

	// Just make sure it runs.
	msdfgen_hello_world();

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

	printf("Entering main loop\n");
	//While application is running

	uint32_t t0 = SDL_GetTicks() - 17;

	Input* input = new Input();

	while( !input->quit_requested() )
	{
//		printf("Looping\n");

		uint32_t t1 = SDL_GetTicks();
		uint32_t dt = t1 - t0;
		t0 = t1;

		//Handle events on queue
		input->poll_events();

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


//	printf("ERRORS >>");
//	printf( SDL_GetError() );
//	printf("<< ERRORS");

	delete input;


	//Free resources and close SDL
	shutdown_sdl_gl();

	return 0;
}
