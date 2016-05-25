#ifndef A_RETRO_UI_RESOURCE_HANDLE_H
#define A_RETRO_UI_RESOURCE_HANDLE_H

#include "Resource_Aux.h"

namespace RetroResource
{
	struct Handle
	{
		unsigned id;
		unsigned index() const { return id & Aux::INDEX_MASK; }
		unsigned generation() const { return ( id >> Aux::INDEX_BITS ) & Aux::GENERATION_MASK; }
	};
}

#endif //A_RETRO_UI_RESOURCE_HANDLE_H
