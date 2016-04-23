
#include <iostream>
#include <vector>
#include <utility>

#include "platform.h"
#include "IRenderer.h"


//Main loop flag
bool quit = false;




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

	//Start up SDL and create window
	printf("Initializing...\n");
	if ( !init_sdl_gl() )
		return 1;

	RendererOpenGL renderer;


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


	// Allocate triangle 1.
	{
		RenderKey render_key;
		RenderData render_data;

		GLfloat vertices[] =
		{
				 0.0f,  0.9f, 0.0f,
				-0.2f,  0.7f, 0.0f,
				 0.2f,  0.7f, 0.0f,
		};
		int n_vertices = sizeof(vertices) / sizeof(GL_FLOAT);
		render_key.RenderOpaque.material_index = 0;
		render_data.vbo_handle = renderer.add_mesh( vertices, n_vertices );

		objects.push_back( { render_key, render_data } );
	}

	// Allocate triangle 2.
	{
		RenderKey render_key;
		RenderData render_data;

		GLfloat vertices[] =
			{
				-0.8f, -0.1f, 0.0f,
				-0.9f, -0.2f, 0.0f,
				-0.7f, -0.2f, 0.0f,
			};
		int n_vertices = sizeof(vertices) / sizeof(GL_FLOAT);
		render_key.RenderOpaque.material_index = 0;
		render_data.vbo_handle = renderer.add_mesh( vertices, n_vertices );

		objects.push_back( { render_key, render_data } );
	}






	//Event handler
	SDL_Event e;

	//Enable text input
	SDL_StartTextInput();

	printf("Entering main loop\n");
	//While application is running
	while( !quit )
	{
//		printf("Looping\n");

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

		for ( auto obj : objects )
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
