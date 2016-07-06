#ifndef A_RETRO_UI_UI_COMPONENTGRID_H
#define A_RETRO_UI_UI_COMPONENTGRID_H

#include <vector>

#include <glm/vec2.hpp>

#include "platform.h"
#include "Ecs_Component.h"

namespace RetroUi
{
	using std::vector;
	using RetroEcs::Entity;

	class ComponentGrid : public RetroEcs::Component< ComponentGrid >
	{
	public:
		struct InstanceData
		{
			vector< Entity >     entity;
			vector< glm::ivec2 > grid_size;
			vector< glm::ivec2 > cell_size;
		} _data;

		ComponentGrid( u32 capacity ) : Component( capacity )
		{
			_data.entity.resize( capacity );
			_data.grid_size.resize( capacity );
			_data.cell_size.resize( capacity );
		}

		void _copy( unsigned i, unsigned i_dst, ComponentGrid::InstanceData& data_dst )
		{
			data_dst.entity[ i_dst ] = _data.entity.at( i );
			data_dst.grid_size[ i_dst ] = _data.grid_size.at( i );
			data_dst.cell_size[ i_dst ] = _data.cell_size.at( i );
		}

		inline glm::ivec2& grid_size( Entity e ) { return get( _data.grid_size, e ); }
		inline void set_grid_size( Entity e, glm::ivec2& size ) { set( _data.grid_size, e, size ); }

		inline glm::ivec2& cell_size( Entity e ) { return get( _data.cell_size, e ); }
		inline void set_cell_size( Entity e, glm::ivec2& size ) { set( _data.cell_size, e, size ); }
	};
}

#endif //A_RETRO_UI_UI_COMPONENTGRID_H
