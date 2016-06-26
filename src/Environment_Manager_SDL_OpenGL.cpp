#include "Environment_Manager_SDL_OpenGL.h"

RetroEnvironment::Manager_SDL_OpenGL::Manager_SDL_OpenGL( SDL_Window* sdl_window, SDL_GLContext sdl_gl_context )
	: _sdl_window( sdl_window )
	, _sdl_gl_context( sdl_gl_context )
{

}

RetroEnvironment::Manager_SDL_OpenGL::~Manager_SDL_OpenGL()
{
	SDL_DestroyWindow( _sdl_window );
	_sdl_window = nullptr;

	// TODO destroy _sdl_gl_context ??
}

void RetroEnvironment::Manager_SDL_OpenGL::swap()
{
	SDL_GL_SwapWindow( _sdl_window );
}

u32 RetroEnvironment::Manager_SDL_OpenGL::get_ticks()
{
	return SDL_GetTicks();
}
