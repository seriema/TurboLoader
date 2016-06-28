#ifndef A_RETRO_UI_UI_SYSTEM_ANIMATOR_H
#define A_RETRO_UI_UI_SYSTEM_ANIMATOR_H

#include <memory>
#include <cmath>
#include <iostream>

#include "platform.h"
#include "math.h"
#include "Ecs_System.h"
#include "Ecs_Time.h"
#include "Ui_ComponentTransform.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemAnimator : public RetroEcs::ISystem
	{
		shared_ptr< RetroEcs::Time >              _time;
		shared_ptr< Context >                     _ui;
		shared_ptr< RetroUi::ComponentTransform > _c_transform;

	public:
		explicit SystemAnimator(
				shared_ptr< RetroEcs::Time > time,
				shared_ptr< Context > ui,
				shared_ptr< RetroUi::ComponentTransform > c_transform )
				: _time( time )
				, _ui( ui )
				, _c_transform( c_transform )
		{
		}

		virtual void tick() override
		{
			for ( u32 i = 0, n = _c_transform->size(); i < n; ++i )
			{
//				float new_x = _c_transform->_data.x[ i ] + 0.5f * _time->dt();
//				_c_transform->_data.x[ i ] = std::fmod( 1100.f + new_x, 2200.f ) - 1100.f;

				float target_scale = _c_transform->_data.entity[ i ].id == _ui->focus.id ? 1.5f : 1.f;
				_c_transform->_data.scale[ i ] = RetroMath::lerp( _c_transform->_data.scale[ i ], target_scale, 0.25f );
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_ANIMATOR_H
