#ifndef A_RETRO_UI_ECS_ENTITY_H
#define A_RETRO_UI_ECS_ENTITY_H

#include "platform.h"
#include "Ecs_Aux.h"

namespace RetroEcs
{
	struct Entity
	{
		u32 id;
		inline u32 index() const { return id & Aux::INDEX_MASK; }
		inline u32 generation() const { return ( id >> Aux::INDEX_BITS ) & Aux::GENERATION_MASK; }
	};
}

#endif //A_RETRO_UI_ECS_ENTITY_H
