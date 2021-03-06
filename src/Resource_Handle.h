#ifndef A_RETRO_UI_RESOURCE_HANDLE_H
#define A_RETRO_UI_RESOURCE_HANDLE_H

#include "platform.h"
#include "Resource_Aux.h"

namespace RetroResource
{
	struct Handle
	{
		u32 id;
		inline u32 index() const { return id & Aux::INDEX_MASK; }
		inline u32 generation() const { return ( id >> Aux::INDEX_BITS ) & Aux::GENERATION_MASK; }
	};
}

#endif //A_RETRO_UI_RESOURCE_HANDLE_H
