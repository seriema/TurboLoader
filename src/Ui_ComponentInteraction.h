#ifndef A_RETRO_UI_UI_COMPONENTINTERACTION_H
#define A_RETRO_UI_UI_COMPONENTINTERACTION_H

#include <vector>

#include "platform.h"
#include "Ecs_Component.h"

namespace RetroUi
{
	using std::vector;
	using RetroEcs::Entity;

	class ComponentInteraction : public RetroEcs::Component< ComponentInteraction >
	{
	public:
		struct InstanceData
		{
			vector< Entity > entity;
			vector< bool >   hover;
			vector< bool >   click;
//			vector< bool >   enter;
//			vector< bool >   leave;
//			vector< bool >   down;
//			vector< bool >   up;
		} _data;

		ComponentInteraction( u32 capacity ) : Component( capacity )
		{
			_data.entity.resize( capacity );
			_data.hover.resize( capacity );
			_data.click.resize( capacity );
		}

		void _copy( unsigned i, unsigned i_dst, ComponentInteraction::InstanceData& data_dst )
		{
			data_dst.entity[ i_dst ] = _data.entity.at( i );
			data_dst.hover[ i_dst ]   = _data.hover.at( i );
			data_dst.click[ i_dst ]   = _data.click.at( i );
		}

		inline const bool hover( Entity e ) { return get( _data.hover, e ); }
		inline void set_hover( Entity e, bool hover ) { set( _data.hover, e, hover ); }

		inline const bool click( Entity e ) { return get( _data.click, e ); }
		inline void set_click( Entity e, bool click ) { set( _data.click, e, click ); }
	};
}

#endif //A_RETRO_UI_UI_COMPONENTINTERACTION_H
