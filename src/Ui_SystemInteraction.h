#ifndef A_RETRO_UI_UI_SYSTEMINTERACTION_H
#define A_RETRO_UI_UI_SYSTEMINTERACTION_H

#include <memory>
#include <cmath>
#include <iostream>

#include "platform.h"
#include "Ecs_System.h"
#include "Ui_ComponentInteraction.h"
#include "Ui_ComponentTransform.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemInteraction : public RetroEcs::ISystem
	{
		shared_ptr< RetroUi::ComponentInteraction > _c_interaction;
		shared_ptr< RetroUi::ComponentTransform >   _c_transform;

	public:
		explicit SystemInteraction(
			shared_ptr< RetroUi::ComponentInteraction > c_interaction,
			shared_ptr< RetroUi::ComponentTransform > c_transform )
			: _c_interaction( c_interaction )
			, _c_transform( c_transform )
		{
		}

		virtual void tick() override
		{
			// Read mouse pos.
			// Is mouse pos intersecting any interaction component?
			// The interaction comp with highest z is the one being hit!
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEMINTERACTION_H
