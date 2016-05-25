#ifndef A_RETRO_UI_RESOURCE_HANDLEMANAGER_H
#define A_RETRO_UI_RESOURCE_HANDLEMANAGER_H

#include <vector>
#include <deque>

#include "platform.h"
#include "Resource_Aux.h"
#include "Resource_handle.h"

namespace RetroResource
{
	class HandleManager
	{
		std::vector<unsigned char> _generation;
		std::deque<unsigned> _free_indices;

	public:
		HandleManager()
		{
			_generation.reserve( Aux::MINIMUM_FREE_INDICES );
			create(); // Reserve Handle.id == 0 for invalid handles.
		}

		inline bool is_alive( Handle h ) const
		{
			return _generation[ h.index() ] == h.generation();
		}

		Handle create()
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
			return Handle { idx + (_generation[ idx ] << Aux::INDEX_BITS) };
		}

		void destroy( Handle r )
		{
			const unsigned idx = r.index();
			++_generation[ idx ];
			_free_indices.push_back( idx );
		}
	};
}

#endif //A_RETRO_UI_RESOURCE_HANDLEMANAGER_H
