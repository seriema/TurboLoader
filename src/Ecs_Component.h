#ifndef A_RETRO_UI_ECS_COMPONENT_H
#define A_RETRO_UI_ECS_COMPONENT_H

#include <vector>
#include <unordered_map>

#include "platform.h"
#include "Ecs_EntityManager.h"

namespace RetroEcs
{
	template< typename T >
	class Component
	{
		EntityManager& _entity_manager;
		u32            _capacity;
		u32            _size;

	protected:
		std::unordered_map< u32, u32 > _index;

		template< typename U >
		inline const U& get( const std::vector<U>& a, Entity e )
		{
			return a[ _index[e.id] ];
		}

		template< typename U >
		inline void set( std::vector<U>& a, Entity e, U& data )
		{
			a[ _index[e.id] ] = data;
		}

	public:
		explicit Component( EntityManager & em, u32 capacity )
			: _entity_manager( em )
			, _capacity( capacity )
			, _size( 0 )
		{
		}

		inline u32 lookup( Entity e )
		{
			return _index.at( e.id );
		}

		Entity create()
		{
			// TODO assert _size doesnt become larger than _capacity!

			auto& data = static_cast< T& >( *this )._data;

			Entity e = _entity_manager.create();
			const u32 i = _size;

			_index.insert( { e.id, i } );
			data._entity[ i ] = e;

			++_size;

			return e;
		}

		void destroy( Entity e )
		{
			auto& data = static_cast< T& >( *this )._data;

			const u32 i = _index.at( e.id );
			const char* name = data._name.at( i );
			const unsigned i_last = _size - 1;
			const Entity e_last = data._resource.at( i_last );

			static_cast< T& >( *this )._copy( i_last, i, data );

			_index[ e_last.id ] =  i;
			_index.erase( e.id );

			--_size;
		}
	};
}

#endif //A_RETRO_UI_ECS_COMPONENT_H
