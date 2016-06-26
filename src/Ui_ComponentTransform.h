#ifndef A_RETRO_UI_UI_COMPONENT_POSITION_H
#define A_RETRO_UI_UI_COMPONENT_POSITION_H

#include <vector>

#include "platform.h"
#include "Ecs_Component.h"

namespace RetroUi
{
	using std::vector;
	using RetroEcs::Entity;

	class ComponentTransform : public RetroEcs::Component< ComponentTransform >
	{
	public:
		struct InstanceData
		{
			vector< Entity > entity;
			vector< float >  x;
			vector< float >  y;
			vector< float >  z;
		} _data;

		ComponentTransform( u32 capacity ) : Component( capacity )
		{
			_data.entity.resize( capacity );
			_data.x.resize( capacity );
			_data.y.resize( capacity );
			_data.z.resize( capacity );
		}

		void _copy( unsigned i, unsigned i_dst, ComponentTransform::InstanceData& data_dst )
		{
			data_dst.entity[ i_dst ] = _data.entity.at( i );
			data_dst.x[ i_dst ]   = _data.x.at( i );
			data_dst.y[ i_dst ]   = _data.y.at( i );
			data_dst.z[ i_dst ]   = _data.z.at( i );
		}

		inline const float x( Entity e ) { return get( _data.x, e ); }
		inline void set_x( Entity e, float x ) { set( _data.x, e, x ); }

		inline const float y( Entity e ) { return get( _data.y, e ); }
		inline void set_y( Entity e, float y ) { set( _data.y, e, y ); }

		inline const float z( Entity e ) { return get( _data.z, e ); }
		inline void set_z( Entity e, float z ) { set( _data.z, e, z ); }
	};
}

#endif //A_RETRO_UI_UI_COMPONENT_POSITION_H
