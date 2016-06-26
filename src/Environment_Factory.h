#ifndef A_RETRO_UI_ENVIRONMENT_FACTORY_H
#define A_RETRO_UI_ENVIRONMENT_FACTORY_H

#include <memory>

namespace RetroEnvironment
{
	class IManager;

	class Factory
	{
	public:
		std::shared_ptr<IManager> create();
		void destroy( std::shared_ptr<IManager> environment_manager );
	};
}

#endif //A_RETRO_UI_ENVIRONMENT_FACTORY_H
