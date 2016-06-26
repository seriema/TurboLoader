#ifndef A_RETRO_UI_APPLICATION_H
#define A_RETRO_UI_APPLICATION_H

#include <vector>

#include "Environment_Manager.h"
#include "Ecs_System.h"

namespace RetroApplication
{
	struct InputContext
	{
		bool quit;
	};

	class Application
	{
		std::shared_ptr< RetroEnvironment::IManager >     _env;
		std::shared_ptr< InputContext >                   _input; // TODO Replace with a command or message!
		std::vector< std::shared_ptr<RetroEcs::ISystem> > _systems;

	public:
		explicit Application(
				std::shared_ptr< RetroEnvironment::IManager > env,
				std::shared_ptr< InputContext > input,
				std::vector< std::shared_ptr<RetroEcs::ISystem> > systems )
			: _env( env )
			, _input( input )
			, _systems( systems )
		{
		}

		void run()
		{
			_input->quit = false;
			while( !_input->quit )
			{
				for ( auto system : _systems )
				{
					system->tick();
				}
			}
		}

		void stop()
		{
			_input->quit = true;
		}
	};
}

#endif //A_RETRO_UI_APPLICATION_H
