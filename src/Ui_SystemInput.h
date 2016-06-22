#ifndef A_RETRO_UI_UI_SYSTEMINPUT_H
#define A_RETRO_UI_UI_SYSTEMINPUT_H

#include <memory>
#include <cmath>
#include <iostream>

#include "platform.h"
#include "Ecs_System.h"
#include "Application.h"
#include "Input.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemInput : public RetroEcs::ISystem
	{
		shared_ptr< Input >                       _input;
		shared_ptr< RetroApplication::StayAlive > _stay_alive;

	public:
		explicit SystemInput(
			shared_ptr< Input > input,
			shared_ptr< RetroApplication::StayAlive > stay_alive )
			: _input( input )
			, _stay_alive( stay_alive )
		{
		}

		virtual void tick() override
		{
			_input->poll_events();
			if ( _input->quit_requested() )
			{
				_stay_alive->value = false;
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEMINPUT_H
