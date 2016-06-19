#ifndef A_RETRO_UI_APPLICATION_H
#define A_RETRO_UI_APPLICATION_H

#include <vector>

#include "Environment_Manager.h"
#include "Ecs_System.h"

namespace RetroApplication
{
	struct StayAlive
	{
		bool value;
	};

	class Application
	{
		std::shared_ptr< RetroEnvironment::IManager >     _env;
		std::shared_ptr< StayAlive >                      _stay_alive; // TODO Replace with a command or message!
		std::vector< std::shared_ptr<RetroEcs::ISystem> > _systems;

	public:
		explicit Application(
				std::shared_ptr< RetroEnvironment::IManager > env,
				std::shared_ptr< StayAlive > stay_alive,
				std::vector< std::shared_ptr<RetroEcs::ISystem> > systems )
			: _env( env )
			, _stay_alive( stay_alive )
			, _systems( systems )
		{
		}

		void run()
		{
			_stay_alive->value = true;
			while( _stay_alive->value )
			{
				for ( auto system : _systems )
				{
					system->tick();
				}
			}
		}

		void stop()
		{
			_stay_alive->value = false;
		}
	};
}

#endif //A_RETRO_UI_APPLICATION_H
