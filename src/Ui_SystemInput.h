#ifndef A_RETRO_UI_UI_SYSTEM_INPUT_H
#define A_RETRO_UI_UI_SYSTEM_INPUT_H

#include <memory>
#include <cmath>
#include <iostream>

#include "platform.h"
#include "Ecs_System.h"
#include "Application.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemInput : public RetroEcs::ISystem
	{
		shared_ptr< RetroApplication::StayAlive > _stay_alive;

		bool _quit;
		int _num_controllers;
		SDL_Joystick* _controller;

		//Analog joystick dead zone
		const int JOYSTICK_DEAD_ZONE = 8000;

	private:
		void handle_keys(SDL_KeyboardEvent event)
		{
			if (event.keysym.sym == SDLK_q)
			{
				_quit = true;
			}
		}

		void handle_controller(SDL_JoyAxisEvent axisEvent, SDL_JoyButtonEvent buttonEvent)
		{
			//Motion on controller 0
			if (axisEvent.which == 0)
			{
				//X axis motion
				if (axisEvent.axis == 0)
				{
					if (axisEvent.value > JOYSTICK_DEAD_ZONE || axisEvent.value < -JOYSTICK_DEAD_ZONE)
					{
						_quit = true;
					}
				}
				//Y axis motion
				else if (axisEvent.axis == 1)
				{
					if (axisEvent.value > JOYSTICK_DEAD_ZONE || axisEvent.value < -JOYSTICK_DEAD_ZONE)
					{
						_quit = true;
					}
				}
			}

			//Button on controller 0
			if (buttonEvent.which == 0)
			{
				if (buttonEvent.state == SDL_PRESSED)
				{
					_quit = true;
				}
			}
		}

	public:
		explicit SystemInput(
			shared_ptr< RetroApplication::StayAlive > stay_alive)
			: _stay_alive( stay_alive )
			, _quit(false)
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
					printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
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
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_QUIT:
					//User requests quit
					_quit = true;
					break;
				case SDL_KEYDOWN:
					handle_keys(e.key);
					break;
				case SDL_JOYAXISMOTION:
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
					handle_controller(e.jaxis, e.jbutton);
					break;
				}
			}

			if (_quit)
			{
				_stay_alive->value = false;
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_INPUT_H
