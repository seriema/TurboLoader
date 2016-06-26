#ifndef A_RETRO_UI_ECS_AUX_H
#define A_RETRO_UI_ECS_AUX_H

#include "platform.h"

namespace RetroEcs
{
	namespace Aux
	{
		const u32 INDEX_BITS = 22;
		const u32 GENERATION_BITS = 8;
		const u32 INDEX_MASK = ( 1 << INDEX_BITS ) - 1;
		const u32 GENERATION_MASK = ( 1 << GENERATION_BITS ) - 1;
		const u32 MINIMUM_FREE_INDICES = 1024;
		const u32 INVALID_HANDLE = 0;
	}
}

#endif //A_RETRO_UI_ECS_AUX_H
