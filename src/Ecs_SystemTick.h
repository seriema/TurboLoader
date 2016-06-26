#ifndef A_RETRO_UI_ECS_SYSTEMTICK_H
#define A_RETRO_UI_ECS_SYSTEMTICK_H

#include <memory>

#include "Ecs_System.h"
#include "Ecs_Time.h"
#include "Environment_Manager.h"

namespace RetroEcs
{
	using std::shared_ptr;

	class SystemTick : public ISystem
	{
		shared_ptr< RetroEnvironment::IManager > _env;
		shared_ptr< RetroEcs::TimeRaw >          _time;

	public:
		SystemTick( shared_ptr< RetroEnvironment::IManager > env, shared_ptr< RetroEcs::TimeRaw > time )
			: _env( env )
			, _time( time )
		{}

		virtual void tick() override
		{
			_time->t0 = _time->t1;
			_time->t1 = _env->get_ticks();
			_time->dt = _time->t1 - _time->t0;
		}
	};
}

#endif //A_RETRO_UI_ECS_SYSTEMTICK_H
