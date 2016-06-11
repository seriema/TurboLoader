#ifndef A_RETRO_UI_UI_COMPONENT_BITMAP_H
#define A_RETRO_UI_UI_COMPONENT_BITMAP_H

#include <vector>

#include "platform.h"
#include "Ecs_Component.h"

namespace RetroUi
{
	using std::vector;
	using RetroEcs::Entity;

	class ComponentBitmap : public RetroEcs::Component< ComponentBitmap >
	{
	public:
		struct InstanceData
		{
			vector< Entity >                entity;
			vector< RetroResource::Handle > bitmap;
			vector< RetroResource::Handle > shader;
		} _data;

		ComponentBitmap( u32 capacity ) : Component( capacity )
		{
			_data.entity.resize( capacity );
			_data.bitmap.resize( capacity );
			_data.shader.resize( capacity );
		}

		void _copy( unsigned i, unsigned i_dst, ComponentBitmap::InstanceData& data_dst )
		{
			data_dst.entity[ i_dst ] = _data.entity.at( i );
			data_dst.bitmap[ i_dst ] = _data.bitmap.at( i );
			data_dst.shader[ i_dst ] = _data.shader.at( i );
		}

		inline const RetroResource::Handle bitmap( Entity e ) { return get( _data.bitmap, e ); }
		inline void set_bitmap( Entity e, RetroResource::Handle bitmap ) { set( _data.bitmap, e, bitmap ); }

		inline const RetroResource::Handle shader( Entity e ) { return get( _data.shader, e ); }
		inline void set_shader( Entity e, RetroResource::Handle shader ) { set( _data.shader, e, shader ); }
	};
}

#endif //A_RETRO_UI_UI_COMPONENT_BITMAP_H
