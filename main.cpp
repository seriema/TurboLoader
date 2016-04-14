
#include <iostream>
#include <SDL2/SDL.h>

#if defined(__APPLE__) || defined(_WIN32)
	#include <SDL2/SDL_opengl.h>
#else // RPI
	#include <SDL2/SDL_opengles2.h>
#endif


#include "platform.h"
#include "shader.h"


//Main loop flag
bool quit = false;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;




Shader* shader = NULL;





void _SDL_GL_SetAttribute(SDL_GLattr attr, int value)
{
  if (SDL_GL_SetAttribute(attr, value) != 0)
  {
    printf("SDL_GL_SetAttribute(%d, %d) failed: %s\n", attr, value, SDL_GetError());
  }
}

bool init()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//hide mouse cursor early
//	SDL_ShowCursor(0);

	_SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	_SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	_SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	_SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	_SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#if defined(__APPLE__) || defined(_WIN32)
	_SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	_SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	_SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
#else // RPI
	_SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	_SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	_SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
#endif

	SDL_DisplayMode dispMode;
	//SDL_GetDesktopDisplayMode(0, &dispMode);
	SDL_GetCurrentDisplayMode(0, &dispMode);

	printf("Screen size: %d %d\n", dispMode.w, dispMode.h);
	//Create window
	gWindow = SDL_CreateWindow(
		"SDL Tutorial",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
#if defined(__APPLE__) || defined(_WIN32)
		960, 480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
#else
		dispMode.w, dispMode.h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
#endif
	);
	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create context
	gContext = SDL_GL_CreateContext( gWindow );
	if( gContext == NULL )
	{
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	SDL_GL_MakeCurrent(gWindow, gContext);

	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

//	glViewport(0, 0, 640, 480);
//	glOrtho(0, 640, 480, 0, -1.0, 1.0);

	//Initialize OpenGL
	if( !initGL() )
	{
		printf( "Unable to initialize OpenGL!\n" );
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

	return true;
}

bool initGL()
{
	//Initialize Projection Matrix
//	glMatrixMode( GL_PROJECTION );
//	glLoadIdentity();

	//Check for error
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		return false;
	}

	//Initialize Modelview Matrix
//	glMatrixMode( GL_MODELVIEW );
//	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		return false;
	}




	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );

	// fragment blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// depth
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDepthMask( GL_TRUE );





	//Initialize clear color
	glClearColor( .5f, 0.f, 0.f, 1.f );

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		return false;
	}

	return true;
}

void handleKeys( unsigned char key, int x, int y )
{
	if( key == 'q' )
	{
		quit = true;
	}
}

void update()
{
	//No per frame update needed
}

void render()
{
	//Clear color buffer
	glClearColor(1.f, 0.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	std::cout << "STARTING IN: " << args[0] << std::endl;

	//Start up SDL and create window
	printf("Initializing...\n");
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		shader = Shader::create( "debug" );
		shader->bind();
		glUseProgram( 0 );
		delete shader;

		printf("Initialized\n");

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

			//Render quad
			render();

			//Update screen
			SDL_GL_SwapWindow( gWindow );
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}
