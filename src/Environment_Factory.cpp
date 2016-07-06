#include <memory>
#include <iostream>

#include "Environment_Factory.h"

#include "platform.h"
#include "Environment_Manager_SDL_OpenGL.h"

using std::cout;
using std::endl;

namespace
{
	std::pair< SDL_GLattr, int > GL_ATTRIBS[] =
	{
		{ SDL_GL_RED_SIZE,     8 },
		{ SDL_GL_GREEN_SIZE,   8 },
		{ SDL_GL_BLUE_SIZE,    8 },
		{ SDL_GL_ALPHA_SIZE,   8 },
		{ SDL_GL_DEPTH_SIZE,   8 },
		{ SDL_GL_DOUBLEBUFFER, 1 },
#if defined(__APPLE__) || defined(_WIN32)
		//{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE }
		{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_COMPATIBILITY },
		{ SDL_GL_CONTEXT_MAJOR_VERSION, 2 },
		{ SDL_GL_CONTEXT_MINOR_VERSION, 1 },
#else // RPI
		{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_ES },
		{ SDL_GL_CONTEXT_MAJOR_VERSION, 2 },
		{ SDL_GL_CONTEXT_MINOR_VERSION, 0 },
#endif
	};
}

std::shared_ptr<RetroEnvironment::IManager> RetroEnvironment::Factory::create()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		cout << "[Environment::Factory::create] SDL could not initialize! SDL Error: '" << SDL_GetError() << "'" << endl;
		return nullptr;
	}

	// TODO On a glorious day we shall move this joystick init poop to a more suitable place.
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1" );
	SDL_InitSubSystem( SDL_INIT_JOYSTICK );
	SDL_JoystickEventState( SDL_ENABLE );

	//hide mouse cursor early
	//	SDL_ShowCursor(0);

	for ( auto attr : GL_ATTRIBS )
	{
		if ( SDL_GL_SetAttribute( attr.first, attr.second ) != 0 )
		{
			cout << "[Environment::Factory::create] FAIL SDL_GL_SetAttribute("
				<< attr.first << ", " << attr.second
				<< ") SDL Error: '" << SDL_GetError() << "'" << endl;
			return nullptr;
		}
	}

	SDL_DisplayMode disp_mode;
	//SDL_GetDesktopDisplayMode(0, &dispMode);
	SDL_GetCurrentDisplayMode( 0, &disp_mode );

	SDL_Window* window = SDL_CreateWindow(
		"SDL Tutorial",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
#if defined(__APPLE__) || defined(_WIN32)
		960, 480,
		SDL_WINDOW_OPENGL |
		SDL_WINDOW_ALLOW_HIGHDPI |
		SDL_WINDOW_BORDERLESS |
		SDL_WINDOW_RESIZABLE
#else
		disp_mode.w, disp_mode.h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
#endif
	);

	int draw_w, draw_h, win_w, win_h;
	SDL_GL_GetDrawableSize( window, &draw_w, &draw_h );
	SDL_GetWindowSize( window, &win_w, &win_h );
	cout << "[Environment::Factory::create] Display mode: " << disp_mode.w << " x " << disp_mode.h << endl;
	cout << "[Environment::Factory::create] Drawable size: " << draw_w << " x " << draw_h << endl;
	cout << "[Environment::Factory::create] Window size: " << win_w << " x " << win_h << endl;


	if( window == nullptr )
	{
		cout << "[Environment::Factory::create] FAIL Window could not be created! SDL Error: '" << SDL_GetError() << "'" << endl;
		return nullptr;
	}

	//Create context
	SDL_GLContext context = SDL_GL_CreateContext( window );
	if( context == nullptr )
	{
		cout << "[Environment::Factory::create] FAIL OpenGL context could not be created! SDL Error: '" << SDL_GetError() << "'" << endl;
		return nullptr;
	}

	SDL_GL_MakeCurrent( window, context );

	gladLoadGLLoader( SDL_GL_GetProcAddress );

	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		cout << "[Environment::Factory::create] Warning: Unable to set VSync! SDL Error: '" << SDL_GetError() << "'" << endl;
	}

	auto environment_manager = std::make_shared<Manager_SDL_OpenGL>( window, context );
	return environment_manager;
}

void RetroEnvironment::Factory::destroy( std::shared_ptr<RetroEnvironment::IManager> environment_manager )
{
	environment_manager.reset();
	SDL_Quit();
}
