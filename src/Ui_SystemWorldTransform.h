#ifndef A_RETRO_UI_UI_SYSTEMWORLDTRANSFORM_H
#define A_RETRO_UI_UI_SYSTEMWORLDTRANSFORM_H

#include <memory>
#include <iostream>

#include "platform.h"

#include "Ecs_System.h"
#include "Ui_ComponentTransform.h"

namespace RetroUi
{
	class SystemWorldTransform : public RetroEcs::ISystem
	{
		shared_ptr< ComponentTransform > _c_transform;

	public:
		explicit SystemWorldTransform( shared_ptr< ComponentTransform > c_transform )
			: _c_transform( c_transform )
		{
		}

		virtual void tick() override
		{
			auto& parent = _c_transform->_data.parent;
			auto& pos = _c_transform->_data.pos;
			auto& world_pos = _c_transform->_data.world_pos;
			auto& scale = _c_transform->_data.scale;
			auto& world_scale = _c_transform->_data.world_scale;

			for ( u32 i = 0, n = _c_transform->size(); i < n; ++i )
			{
				if ( parent[i] == _c_transform->INVALID_INDEX )
				{
					world_pos[ i ] = pos[ i ];
					world_scale[ i ] = scale[ i ];
				}
				else
				{
					world_pos[ i ] = pos[ i ] + world_pos[ parent[i] ];
					world_scale[ i ] = scale[ i ] * world_scale[ parent[i] ];
				}
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEMWORLDTRANSFORM_H
