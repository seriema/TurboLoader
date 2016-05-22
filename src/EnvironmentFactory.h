#ifndef A_RETRO_UI_ENVIRONMENTFACTORY_H
#define A_RETRO_UI_ENVIRONMENTFACTORY_H

#include <memory>

#include "platform.h"
#include "EnvironmentManager.h"


class IEnvironmentFactory
{
public:
	virtual ~IEnvironmentFactory() {}
	virtual std::shared_ptr< IEnvironmentManager > create_environment_manager() = 0;
	virtual void destroy_environment_manager( std::shared_ptr< IEnvironmentManager > environment_manager ) = 0;
};


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


class EnvironmentFactory_SDL_OpenGL : public IEnvironmentFactory
{
public:
	EnvironmentFactory_SDL_OpenGL()// const EnvironmentSettings& settings )
	{
	}

	//virtual ~EnvironmentFactory_SDL_OpenGL() override {}

	virtual std::shared_ptr< IEnvironmentManager > create_environment_manager() override
	{
		if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
			return nullptr;
		}

		//hide mouse cursor early
		//	SDL_ShowCursor(0);

		for ( auto attr : GL_ATTRIBS )
		{
			if ( SDL_GL_SetAttribute( attr.first, attr.second ) != 0 )
			{
				printf( "SDL_GL_SetAttribute(%d, %d) failed: %s\n", attr.first, attr.second, SDL_GetError() );
				return nullptr;
			}
		}

		SDL_DisplayMode dispMode;
		//SDL_GetDesktopDisplayMode(0, &dispMode);
		SDL_GetCurrentDisplayMode(0, &dispMode);

		printf("Screen size: %d %d\n", dispMode.w, dispMode.h);

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
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			return nullptr;
		}

		//Create context
		SDL_GLContext context = SDL_GL_CreateContext( window );
		if( context == nullptr )
		{
			printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
			return nullptr;
		}

		SDL_GL_MakeCurrent( window, context );

		gladLoadGLLoader( SDL_GL_GetProcAddress );

		//Use Vsync
		if( SDL_GL_SetSwapInterval( 1 ) < 0 )
		{
			printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
		}

		auto environment_manager = std::make_shared< EnvironmentManager_SDL_OpenGL >( window, context );
		return environment_manager;
	}

	virtual void destroy_environment_manager( std::shared_ptr< IEnvironmentManager > environment_manager ) override
	{
// TODO		delete environment_manager;
		SDL_Quit();
	}
};

#endif //A_RETRO_UI_ENVIRONMENTFACTORY_H
