#ifndef A_RETRO_UI_UI_CONTEXT_H
#define A_RETRO_UI_UI_CONTEXT_H

#include "Ecs_Entity.h"

namespace RetroUi
{
	struct Context
	{
		RetroEcs::Entity scene; // Top-most element, unfocusable.
		RetroEcs::Entity focus; // All focusable ui elements always has a parent.
	};
}

#endif //A_RETRO_UI_UI_CONTEXT_H
