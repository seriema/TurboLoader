/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_opengl.h>

#include <bgfx/bgfxplatform.h> // it must be included after SDL to enable SDL integration code path.
#include <bgfx/bgfx.h>
#include <bx/uint32_t.h>

#include "logo.h"


//Screen dimension constants
unsigned int display_width = 800;
unsigned int display_height = 600;

//Main loop flag
bool quit = false;


//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

////OpenGL context
//SDL_GLContext gContext;

bool init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	printf("SDL_Init done\n");

	//Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	printf("SD_GL_SetAttribute done\n");


	SDL_DisplayMode dispMode;
	SDL_GetDesktopDisplayMode(0, &dispMode);
	if(display_width == 0)
		display_width = dispMode.w;
	if(display_height == 0)
		display_height = dispMode.h;

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		display_width, display_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	printf("SDL_CreateWindow done\n");

	bgfx::sdlSetWindow(gWindow);
	printf("bgfx::sdlSetWindow done\n");

	if (!bgfx::init()) {
		printf("Unable to initialize bgfx!\n");
	}
	printf("bgfx::init done\n");

	// Enable debug text.
	bgfx::setDebug(BGFX_DEBUG_TEXT);
	printf("bgfx::setDebug done\n");

	bgfx::reset(display_width, display_height, BGFX_RESET_VSYNC);
	printf("bgfx::reset done\n");

	// Set view 0 clear state.
	bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
	);
	printf("bgfx::setViewClear done\n");

	return true;
}

void render() {
	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, display_width, display_height);

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);

	// Use debug font to print information about this example.
	bgfx::dbgTextClear();
	bgfx::dbgTextImage(bx::uint16_max(display_width/2/8, 20)-20
			, bx::uint16_max(display_height/2/16, 6)-6
			, 40
			, 12
			, s_logo
			, 160
	);
	bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
	bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");

	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	bgfx::frame();
}

void close() {
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char *args[]) {
	printf("Attempting to initialize...\n");

	//Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		printf("Initializing!\n");

		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit) {
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0) {
				//User requests quit
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}

			render();
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}
