#ifndef A_RETRO_UI_ECS_TIME_H
#define A_RETRO_UI_ECS_TIME_H

#include <memory>

#include "Environment_Manager.h"

namespace RetroEcs
{
	struct TimeRaw
	{
		u32 time;
		u32 t0;
		u32 t1;
		u32 dt;
	};

	class Time
	{
		std::shared_ptr< TimeRaw > _time;
	public:
		Time( std::shared_ptr< TimeRaw > time ) : _time( time ) {}
		inline u32 time() { return _time->time; }
		inline u32 t0() { return _time->t0; }
		inline u32 t1() { return _time->t1; }
		inline u32 dt() { return _time->dt; }
	};
}

#endif //A_RETRO_UI_ECS_TIME_H
