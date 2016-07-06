#ifndef A_RETRO_UI_UI_COMPONENT_POSITION_H
#define A_RETRO_UI_UI_COMPONENT_POSITION_H

#include <vector>

#include <glm/vec3.hpp>

#include "platform.h"
#include "Ecs_Component.h"

namespace RetroUi
{
	using std::vector;
	using glm::vec3;
	using RetroEcs::Entity;

	class ComponentTransform : public RetroEcs::Component< ComponentTransform >
	{
	public:
		struct InstanceData
		{
			vector< Entity > entity;
			vector< vec3 >   pos;
			vector< vec3 >   world_pos;
			vector< float >  scale;
			vector< float >  world_scale;
			vector< u32 >    parent;
			vector< u32 >    first_child;
			vector< u32 >    next_sibling; // TODO performance test if there's any benefit of keeping siblings sorted together.
			vector< u32 >    prev_sibling; // TODO performance test if there's any benefit of keeping siblings sorted together.
		} _data;

		ComponentTransform( u32 capacity ) : Component( capacity )
		{
			_data.entity.resize( capacity );
			_data.pos.resize( capacity );
			_data.world_pos.resize( capacity );
			_data.scale.resize( capacity );
			_data.world_scale.resize( capacity );
			_data.parent.resize( capacity );
			_data.first_child.resize( capacity );
			_data.next_sibling.resize( capacity );
			_data.prev_sibling.resize( capacity );

			std::fill( _data.parent.begin(), _data.parent.end(), INVALID_INDEX );
			std::fill( _data.first_child.begin(), _data.first_child.end(), INVALID_INDEX );
			std::fill( _data.next_sibling.begin(), _data.next_sibling.end(), INVALID_INDEX );
			std::fill( _data.prev_sibling.begin(), _data.prev_sibling.end(), INVALID_INDEX );
		}

		void _copy( unsigned i, unsigned i_dst, ComponentTransform::InstanceData& data_dst )
		{
			data_dst.entity[ i_dst ] = _data.entity.at( i );
			data_dst.pos[ i_dst ]   = _data.pos.at( i );
			data_dst.world_pos[ i_dst ]   = _data.world_pos.at( i );
			data_dst.scale[ i_dst ]   = _data.scale.at( i );
			data_dst.world_scale[ i_dst ]   = _data.world_scale.at( i );
			data_dst.parent[ i_dst ]   = _data.parent.at( i );
			data_dst.first_child[ i_dst ]   = _data.first_child.at( i );
			data_dst.next_sibling[ i_dst ]   = _data.next_sibling.at( i );
			data_dst.prev_sibling[ i_dst ]   = _data.prev_sibling.at( i );
		}

		inline vec3& pos( Entity e ) { return get( _data.pos, e ); }
		inline void set_pos( Entity e, const vec3& pos ) { set( _data.pos, e, pos ); }

		inline vec3& world_pos( Entity e ) { return get( _data.world_pos, e ); }
		inline void set_world_pos( Entity e, vec3& pos ) { set( _data.world_pos, e, pos ); }

		inline float scale( Entity e ) { return get( _data.scale, e ); }
		inline void set_scale( Entity e, float scale ) { set( _data.scale, e, scale ); }

		inline float world_scale( Entity e ) { return get( _data.world_scale, e ); }
		inline void set_world_scale( Entity e, float world_scale ) { set( _data.world_scale, e, world_scale ); }

		inline u32 parent( Entity e ) { return get( _data.parent, e ); }
		inline void set_parent( Entity e, u32 parent ) { set( _data.parent, e, parent ); }

		inline u32 first_child( Entity e ) { return get( _data.first_child, e ); }
		inline void set_first_child( Entity e, u32 first_child ) { set( _data.first_child, e, first_child ); }

		inline u32 next_sibling( Entity e ) { return get( _data.next_sibling, e ); }
		inline void set_next_sibling( Entity e, u32 next_sibling ) { set( _data.next_sibling, e, next_sibling ); }

		inline u32 prev_sibling( Entity e ) { return get( _data.prev_sibling, e ); }
		inline void set_prev_sibling( Entity e, u32 prev_sibling ) { set( _data.prev_sibling, e, prev_sibling ); }

		void append_child( Entity e_parent, Entity e_child )
		{
			u32 parent = lookup( e_parent );
			u32 child = lookup( e_child );
			u32 last_child = _data.first_child[ parent ];

			if ( last_child == INVALID_INDEX )
			{
				_data.parent[ child ] = parent;
				_data.first_child[ parent ] = child;
			}
			else
			{
				u32 __MAX = 100;
				while ( _data.next_sibling[ last_child ] != INVALID_INDEX )
				{
					last_child = _data.next_sibling[ last_child ];

					if ( !--__MAX )
					{
						std::cout << "[RetroUi::ComponentTransform::append_child] FAILED TO APPEND CHILD!" << std::endl;
						return;
					}
				}

				_data.parent[ child ] = parent;
				_data.next_sibling[ last_child ] = child;
				_data.prev_sibling[ child ] = last_child;
			}
		}
	};
}

#endif //A_RETRO_UI_UI_COMPONENT_POSITION_H
