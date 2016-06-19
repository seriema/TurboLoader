#ifndef A_RETRO_UI_ENVIRONMENTMANAGER_H
#define A_RETRO_UI_ENVIRONMENTMANAGER_H

#include "platform.h"

namespace RetroEnvironment
{
	class IManager
	{
	public:
		virtual ~IManager() {};
		virtual void swap() = 0;
		virtual u32 get_ticks() = 0;
	};
}

#endif //A_RETRO_UI_ENVIRONMENTMANAGER_H
