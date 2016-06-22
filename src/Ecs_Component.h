#ifndef A_RETRO_UI_ECS_COMPONENT_H
#define A_RETRO_UI_ECS_COMPONENT_H

#include <vector>
#include <unordered_map>

#include "platform.h"
#include "Ecs_Entity.h"

#define GETSET( TYPE, COMP ) \
	inline const TYPE& COMP( Entity e ) { return get( _data._##COMP, e ); } \
	inline void set_##COMP( Entity e, TYPE COMP ) { set( _data._##COMP, e, COMP ); } \

namespace RetroEcs
{
	template< typename T >
	class Component
	{
		u32 _capacity;
		u32 _size;

	protected:
		std::unordered_map< u32, u32 > _index;

		template< typename U >
		inline const U& get( const std::vector<U>& a, Entity e )
		{
			return a[ _index.at(e.id) ];
		}

		template< typename U >
		inline void set( std::vector<U>& a, Entity e, U& data )
		{
			a[ _index.at(e.id) ] = data;
		}

	public:
		explicit Component( u32 capacity )
			: _capacity( capacity )
			, _size( 0 )
		{
		}

		inline u32 size()
		{
			return _size;
		}

		inline u32 lookup( Entity e )
		{
			return _index.at( e.id );
		}

		u32 create( Entity e )
		{
			// TODO assert _size doesnt become larger than _capacity!

			const u32 i = _size;
			++_size;

			_index.insert( { e.id, i } );
			static_cast< T& >( *this )._data.entity[ i ] = e;

			return i;
		}

		void destroy( Entity e )
		{
			// TODO In debug mode this should always clear all memory!

			auto& data = static_cast< T& >( *this )._data;

			const u32 i = _index.at( e.id );
			const unsigned i_last = _size - 1;
			const Entity e_last = data.entity[ i_last ];

			static_cast< T& >( *this )._copy( i_last, i, data );

			_index[ e_last.id ] =  i;
			_index.erase( e.id );

			--_size;
		}
	};
}

#endif //A_RETRO_UI_ECS_COMPONENT_H
