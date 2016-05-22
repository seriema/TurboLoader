#ifndef A_RETRO_UI_ENVIRONMENTMANAGER_H
#define A_RETRO_UI_ENVIRONMENTMANAGER_H

#include "platform.h"

//enum class EnvironmentType : uint32_t
//{
//	SDL    = 0x01,
//	OpenGL = 0x02,
//	// = 0x04,
//	// = 0x08,
//	// = 0x10,
//	// = 0x20,
//	// = 0x40,
//};
//
//inline EnvironmentType operator| ( EnvironmentType lhs, EnvironmentType rhs )
//{
//	return static_cast< EnvironmentType >(
//		static_cast< std::underlying_type< EnvironmentType >::type >(lhs) |
//		static_cast< std::underlying_type< EnvironmentType >::type >(rhs)
//	);
//}

//struct EnvironmentSettings
//{
//	std::uint32_t window_w;
//	std::uint32_t window_h;
//	std::uint32_t window_flags;
//
////	EnvironmentType environment_type;
//};


class IEnvironmentManager
{
public:
	virtual ~IEnvironmentManager() {};
	virtual void swap() = 0;
	virtual uint32_t get_ticks() = 0;
};


class EnvironmentManager_SDL_OpenGL : public IEnvironmentManager
{
public:
	EnvironmentManager_SDL_OpenGL( SDL_Window* sdl_window, SDL_GLContext sdl_gl_context )
			: _sdl_window( sdl_window )
			, _sdl_gl_context( sdl_gl_context )
	{
	}

	virtual ~EnvironmentManager_SDL_OpenGL() override
	{
		SDL_DestroyWindow( _sdl_window );
		_sdl_window = nullptr;

		// TODO destroy _sdl_gl_context ??
	}

	virtual void swap() override
	{
		SDL_GL_SwapWindow( _sdl_window );
	}

	virtual uint32_t get_ticks() override
	{
		return SDL_GetTicks();
	}

private:
	SDL_Window*      _sdl_window;
	SDL_GLContext    _sdl_gl_context;
};

#endif //A_RETRO_UI_ENVIRONMENTMANAGER_H
