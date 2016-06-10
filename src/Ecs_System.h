#ifndef A_RETRO_UI_ECS_SYSTEM_H
#define A_RETRO_UI_ECS_SYSTEM_H

#include "platform.h"

namespace RetroEcs
{
	class System
	{
	public:
		virtual ~System() {}
		virtual void init() {}
		virtual void exit() {}
		virtual void tick( u32 dt ) = 0;
	};
}

#endif //A_RETRO_UI_ECS_SYSTEM_H
