#ifndef A_RETRO_UI_ENVIRONMENT_MANAGER_SDL_OPENGL_H
#define A_RETRO_UI_ENVIRONMENT_MANAGER_SDL_OPENGL_H

#include "platform.h"
#include "Environment_Manager.h"

namespace RetroEnvironment
{
	class Manager_SDL_OpenGL : public IManager
	{
	public:
		Manager_SDL_OpenGL( SDL_Window* sdl_window, SDL_GLContext sdl_gl_context );
		virtual ~Manager_SDL_OpenGL() override;
		virtual void swap() override;
		virtual u32 get_ticks() override;
		virtual glm::vec2 resolution() override;
	private:
		SDL_Window*      _sdl_window;
		SDL_GLContext    _sdl_gl_context;
	};

}

#endif //A_RETRO_UI_ENVIRONMENT_MANAGER_SDL_OPENGL_H
