#ifndef A_RETRO_UI_ECS_ENTITYMANAGER_H
#define A_RETRO_UI_ECS_ENTITYMANAGER_H

#include <vector>
#include <deque>

#include "platform.h"
#include "Ecs_Aux.h"
#include "Ecs_Entity.h"

namespace RetroEcs
{
	class EntityManager
	{
		std::vector<unsigned char> _generation;
		std::deque<unsigned>       _free_indices;

	public:
		EntityManager()
		{
			_generation.reserve( Aux::MINIMUM_FREE_INDICES );
			create(); // Reserve Entity.id == 0 for invalid entities.
		}

		inline bool is_alive( Entity e ) const
		{
			return _generation[ e.index() ] == e.generation();
		}

		Entity create()
		{
			unsigned idx;
			if ( _free_indices.size() > Aux::MINIMUM_FREE_INDICES )
			{
				idx = _free_indices.front();
				_free_indices.pop_front();
			}
			else
			{
				_generation.push_back( 0 );
				idx = _generation.size() - 1;
			}
			return Entity { idx + (_generation[ idx ] << Aux::INDEX_BITS) };
		}

		void destroy( Entity e )
		{
			const unsigned idx = e.index();
			++_generation[ idx ];
			_free_indices.push_back( idx );
		}
	};
}

#endif //A_RETRO_UI_ECS_ENTITYMANAGER_H
