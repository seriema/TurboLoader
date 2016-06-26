#ifndef A_RETRO_UI_ECS_SYSTEM_H
#define A_RETRO_UI_ECS_SYSTEM_H

#include "platform.h"

namespace RetroEcs
{
	class ISystem
	{
	public:
		virtual ~ISystem() {}
		virtual void tick() = 0;
	};
}

#endif //A_RETRO_UI_ECS_SYSTEM_H
