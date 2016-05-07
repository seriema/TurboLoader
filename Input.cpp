// VS requires this stdio.h for printf: https://msdn.microsoft.com/en-us/library/bb531344.aspx
#include <stdio.h>

#include <SDL_events.h>

#include "Input.h"

Input::Input()
	: _quit( false )
	, _num_controllers( 0 )
	, _controller( NULL )
{
	init_controller();
}

Input::~Input()
{
	if( _controller != NULL )
	{
		//Close game controller
		SDL_JoystickClose( _controller );
	}
}

void Input::init_controller()
{
	_num_controllers = SDL_NumJoysticks();

	if ( _num_controllers > 0 )
	{
		//Load joystick
		// Note: We currently only support one controller!
		_controller = SDL_JoystickOpen( 0 );
		if ( _controller == NULL )
		{
			printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
		}
	}
}

void Input::poll_events()
{
	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
		switch ( e.type )
		{
			case SDL_QUIT:
				//User requests quit
				_quit = true;
				break;
			case SDL_KEYDOWN:
				handle_keys( e.key );
				break;
			case SDL_JOYAXISMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				handle_controller( e.jaxis, e.jbutton );
				break;
		}
	}
}

bool Input::quit_requested()
{
	return _quit;
}

void Input::handle_keys( SDL_KeyboardEvent event )
{
	if ( event.keysym.sym == SDLK_ESCAPE )
	{
		_quit = true;
	}
}

void Input::handle_controller( SDL_JoyAxisEvent axisEvent, SDL_JoyButtonEvent buttonEvent )
{
	//Motion on controller 0
	if ( axisEvent.which == 0 )
	{
		//X axis motion
		if ( axisEvent.axis == 0 )
		{
			if ( axisEvent.value > JOYSTICK_DEAD_ZONE || axisEvent.value < -JOYSTICK_DEAD_ZONE )
			{
				_quit = true;
			}
		}
		//Y axis motion
		else if ( axisEvent.axis == 1 )
		{
			if ( axisEvent.value > JOYSTICK_DEAD_ZONE || axisEvent.value < -JOYSTICK_DEAD_ZONE )
			{
				_quit = true;
			}
		}
	}

	//Button on controller 0
	if ( buttonEvent.which == 0 )
	{
		if ( buttonEvent.state == SDL_PRESSED )
		{
			_quit = true;
		}
	}
}
