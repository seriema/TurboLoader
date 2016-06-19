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

	SDL_DisplayMode dispMode;
	//SDL_GetDesktopDisplayMode(0, &dispMode);
	SDL_GetCurrentDisplayMode(0, &dispMode);

	cout << "[Environment::Factory::create] Screen size: " << dispMode.w << " x " << dispMode.h << endl;

	SDL_Window* window = SDL_CreateWindow(
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
