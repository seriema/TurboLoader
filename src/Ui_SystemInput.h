#ifndef A_RETRO_UI_UI_SYSTEM_INPUT_H
#define A_RETRO_UI_UI_SYSTEM_INPUT_H

#include <memory>   // std::shared_ptr
#include <cstring>  // std::memset
#include <cstdlib>  // std::abs
#include <limits>   // std::numeric_limits::max
#include <iostream> // std::cout, std::endl

#include "platform.h"
#include "Ecs_System.h"
#include "Application.h"
#include "Math.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemInput : public RetroEcs::ISystem
	{
		shared_ptr< RetroApplication::InputContext > _input;

		int _num_controllers;
		SDL_Joystick* _controller;

		const float JOY_DEADZONE = .2f;
		const float JOY_SCALE = 1.f / std::numeric_limits<Sint16>::max();
		const float JOY_INTERVAL_INVERSE = 1.f / (1.f - JOY_DEADZONE);
		const float JOY_INTERVAL_DEADZONE = JOY_DEADZONE * JOY_INTERVAL_INVERSE;
		const float JOY_INTERVAL_SCALE = JOY_SCALE * JOY_INTERVAL_INVERSE;

	private:
		inline float read_axis( float v )
		{
			return RetroMath::sign(v) * std::max( 0.f, JOY_INTERVAL_SCALE * std::abs(v) - JOY_INTERVAL_DEADZONE );
		}

		void handle_keys( SDL_KeyboardEvent event, float mult )
		{
			switch (event.keysym.sym)
			{
				case SDLK_q:      _input->quit   = true; break;

				case SDLK_SPACE:  _input->select = true; break;
				case SDLK_RETURN: _input->start  = true; break;

				case SDLK_RIGHT:  _input->horizontal = mult; break;
				case SDLK_LEFT:   _input->horizontal = -mult; break;
				case SDLK_UP:     _input->vertical = mult; break;
				case SDLK_DOWN:   _input->vertical = -mult; break;
			}
		}

		void handle_controller_axis( SDL_JoyAxisEvent axisEvent )
		{
			std::cout << "[RetroUi::SystemInput::handle_controller] " << axisEvent.which << "::" << (int)axisEvent.axis << "::" << axisEvent.value << std::endl;

			/* TODO Retardness overload! On apple (win also?) both controllers counts as first controller:
			 * axis0 is x-axis joy0
			 * axis1 is x-axis joy1
			 * axis2 is y-axis joy0
			 * axis3 is y-axis joy1
			 */

			//Motion on controller 0
			if ( axisEvent.which == 0 )
			{
#if defined(__APPLE__) || defined(_WIN32)
				if ( axisEvent.axis < 2u )
#else
				if ( axisEvent.axis == 0u )
#endif
				{
					_input->horizontal = read_axis( (float)axisEvent.value );
				}
#if defined(__APPLE__) || defined(_WIN32)
				else if ( axisEvent.axis < 4u )
#else
				else if ( axisEvent.axis == 1u )
#endif
				{
					_input->vertical = -read_axis( (float)axisEvent.value );
				}
			}
		}

		void handle_controller_button( SDL_JoyButtonEvent buttonEvent, float mult )
		{
			//Button on controller 0
			if ( buttonEvent.which == 0 )
			{
				if ( buttonEvent.state == SDL_PRESSED )
				{
					_input->quit = true;
				}
			}
		}

	public:
		explicit SystemInput(
			shared_ptr< RetroApplication::InputContext > input)
			: _input( input )
			, _num_controllers(0)
			, _controller(NULL)
		{
			_num_controllers = SDL_NumJoysticks();

			if (_num_controllers > 0)
			{
				//Load joystick
				// Note: We currently only support one controller!
				_controller = SDL_JoystickOpen(0);
				if (_controller == NULL)
				{
					std::cout << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
		}

		~SystemInput()
		{
			if (_controller != NULL)
			{
				//Close game controller
				SDL_JoystickClose(_controller);
			}
		}

		virtual void tick() override
		{
			//memset( &*_input, 0, sizeof(*_input) );

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_QUIT:
					//User requests quit
					_input->quit = true;
					break;
				case SDL_KEYDOWN:
					handle_keys( e.key, 1.f );
					break;
				case SDL_KEYUP:
					handle_keys( e.key, 0.f );
					break;
				case SDL_JOYAXISMOTION:
					handle_controller_axis( e.jaxis );
					break;
				case SDL_JOYBUTTONDOWN:
					handle_controller_button( e.jbutton, 1.f );
					break;
				case SDL_JOYBUTTONUP:
					handle_controller_button( e.jbutton, 0.f );
					break;
				}
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_INPUT_H
