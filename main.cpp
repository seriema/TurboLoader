#include <string>
#include <iostream>

#include "shader.h"

#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif


//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Global variables
std::string programName = "A RETRO UI";

//Main loop flag
bool quit = false;


//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Extra init stuff
bool postInit();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders stuff to the screen
void render();

//Setup buffer objects
bool setupBufferObjects();

//Frees media and shuts down SDL
void close();

//Main application loop
void mainLoop(SDL_Event &e);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

// Our object has 12 points
const uint32_t points = 12;

// Each poin has three values ( x, y, z)
const uint32_t floatsPerPoint = 3;

// Each color has 4 values ( red, green, blue, alpha )
const uint32_t floatsPerColor = 4;

// This is the object we'll draw ( a simple diamond
const GLfloat diamond[points][floatsPerPoint] = {
		{  0.2,  0.2,  0.5 }, // Top right
		{ -0.2,  0.2,  0.5 }, // Top left
		{  0.0,  0.0,  0.5 }, // Center

		{  0.2,  0.2,  0.5 }, // Top right
		{  0.2, -0.2,  0.5 }, // Bottom right
		{  0.0,  0.0,  0.5 }, // Center

		{ -0.2, -0.2,  0.5 }, // Bottom left
		{  0.2, -0.2,  0.5 }, // Bottom right
		{  0.0,  0.0,  0.5 }, // Center

		{ -0.2, -0.2,  0.5 }, // Bottom left
		{ -0.2,  0.2,  0.5 }, // Top left
		{  0.0,  0.0,  0.5 }, // Center
};

// This is the object we'll draw ( a simple diamond
const GLfloat colors[points][floatsPerColor] = {
		{ 0.5, 0.5, 0.5, 1.0f }, // Top right
		{ 0.5, 0.5, 0.5, 1.0f }, // Bottom right
		{ 0.0, 0.0, 0.0, 1.0f }, // Center

		{ 0.5, 0.5, 0.5, 1.0f }, // Top left
		{ 0.5, 0.5, 0.5, 1.0f }, // Top right
		{ 0.0, 0.0, 0.0, 1.0f }, // Center

		{ 0.5, 0.5, 0.5, 1.0f }, // Bottom left
		{ 0.5, 0.5, 0.5, 1.0f }, // Bottom right
		{ 0.0, 0.0, 0.0, 1.0f }, // Center

		{ 0.5, 0.5, 0.5, 1.0f }, // Bottom left
		{ 0.5, 0.5, 0.5, 1.0f }, // Top left
		{ 0.0, 0.0, 0.0, 1.0f }, // Center
};

// Create variables for storing the ID of our VAO and VBO
GLuint vbo[2], vao[1];

// The positons of the position and color data within the VAO
const uint32_t positionAttributeIndex = 0, colorAttributeIndex = 1;

// Our wrapper to simplify the shader code
Shader shader;

bool init()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}


	//Initialize OpenGL
	if( !initGL() )
	{
		printf( "Unable to initialize OpenGL!\n" );
		return false;
	}


	//Create context
	gContext = SDL_GL_CreateContext( gWindow );
	if( gContext == NULL )
	{
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool initGL()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

bool postInit()
{
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear(GL_COLOR_BUFFER_BIT);

	//Check for error
	if( glGetError() != GL_NO_ERROR )
	{
		return false;
	}

	SDL_GL_SwapWindow(gWindow);

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
	// First, render a square without any colors ( all vertexes will be black )
	// ===================
	// Make our background black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Invoke glDrawArrays telling that our data is a line loop and we want to draw 4 vertexes
	glDrawArrays(GL_TRIANGLES, 0, points);

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(gWindow);
}

bool setupBufferObjects()
{
	// Generate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(2, vbo);

	// Generate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, vao);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[0]);

	// Colors
	// =======================
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerColor) * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Positions
	// ===================
	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerPoint) * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable our attributes within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);
	glEnableVertexAttribArray(colorAttributeIndex);

	// Set up shader ( will be covered in the next part )
	// ===================
	if (!shader.Init())
		return false;

	shader.UseProgram();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void close()
{
	// Cleanup all the things we bound and allocated
	shader.CleanUp();

	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);

	// Delete our OpengL context
	SDL_GL_DeleteContext(gContext);

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void mainLoop(SDL_Event &e) {
	//While application is running
	while( !quit )
	{
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

			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = false;
						break;
					case SDLK_r:
						// Cover with red and update
						glClearColor(1.0, 0.0, 0.0, 1.0);
						glClear(GL_COLOR_BUFFER_BIT);
						SDL_GL_SwapWindow(gWindow);
						break;
					case SDLK_g:
						// Cover with green and update
						glClearColor(0.0, 1.0, 0.0, 1.0);
						glClear(GL_COLOR_BUFFER_BIT);
						SDL_GL_SwapWindow(gWindow);
						break;
					case SDLK_b:
						// Cover with blue and update
						glClearColor(0.0, 0.0, 1.0, 1.0);
						glClear(GL_COLOR_BUFFER_BIT);
						SDL_GL_SwapWindow(gWindow);
						break;
					default:
						break;
				}
			}
		}

		//Render quad
		render();

		//Update screen
		SDL_GL_SwapWindow( gWindow );
	}
}

int main(int argc, char* args[] )
{
	//Start up SDL and create window
	std::cout << "Start up SDL" << std::endl;
	if( !init() )
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	std::cout << "Clear our buffer background" << std::endl;
	if (!postInit()) {
		return -1;
	}

	std::cout << "Setting up VBO + VAO..." << std::endl;
	if (!setupBufferObjects())
	{
		return -1;
	}

	//Event handler
	SDL_Event e;

	//Enable text input
	SDL_StartTextInput();

	//Start main application loop
//	mainLoop(e);
	render();
	std::cout << "Rendering done! Press ENTER to quit.\n";
	std::cin.ignore();

	//Disable text input
	SDL_StopTextInput();

	//Free resources and close SDL
	close();

	return 0;
}
