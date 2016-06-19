#ifndef A_RETRO_UI_RESOURCE_MESHLOADER_H
#define A_RETRO_UI_RESOURCE_MESHLOADER_H

#include <string>
#include <vector>

#include "platform.h"
#include "Resource_Handle.h"
#include "Resource_HandleManager.h"
#include "Resource_MeshCollection.h"

namespace RetroResource
{
	class MeshLoader
	{
		HandleManager&  _handle_manager;
		MeshCollection& _meshes;

	public:
		MeshLoader( HandleManager& handle_manager, MeshCollection& meshes )
			: _handle_manager( handle_manager )
			, _meshes( meshes )
		{}

		u32 load( const std::string* names, const std::string* paths, Handle* handles, const u32 size = 1 )
		{
			return 0;
		}

		Handle load_single( const std::string name, float* vertices, const u32 vertices_n, u32* indices, const u32 indices_n )
		{
			Handle handle = _handle_manager.create();

			u32 size = _meshes.handle.size();

			_meshes.handle_index.insert( { handle.id, size } );
			_meshes.name_index.insert( { name, size } );

			_meshes.handle.push_back( handle );
			_meshes.name.push_back( name );
			_meshes.path.push_back( "INTERNAL" );
			_meshes.mesh.push_back( RetroResource::Mesh { vertices_n, indices_n, vertices, indices });

			return handle;
		}

		u32 unload( Handle* handles, const u32 size = 1 )
		{
			return 0;
		}
	};
}

#endif //A_RETRO_UI_RESOURCE_MESHLOADER_H
