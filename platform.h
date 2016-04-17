#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(__APPLE__)
	#include <SDL.h>
	#include <SDL2/SDL_opengl.h>
	#include <OpenGL/gl3.h>
#elif defined(_WIN32)
	#include <SDL.h>
	#include <glad/glad.h>
#else // RPI
	#include <SDL.h>
	#include <SDL2/SDL_opengles2.h>
	#include <GLES2/gl2.h>
#endif

#endif //PLATFORM_H
