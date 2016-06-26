#ifndef A_RETRO_UI_GRAPHICS_MESHMANAGER_OPENGL_H
#define A_RETRO_UI_GRAPHICS_MESHMANAGER_OPENGL_H

#include <unordered_map>
#include <queue>
#include <string>

#include "platform.h"

#include "Graphics_MeshManager.h"
#include "Resource_MeshCollection.h"

namespace RetroGraphics
{
	struct Mesh
	{
		union
		{
			u32 raw[ 2 ];
			struct { u32 vbo, ibo; };
		};
	};

	class MeshManager_OpenGL : public IMeshManager
	{
		RetroResource::MeshCollection & _meshes;
		std::unordered_map< u32, Mesh > _handles;

	public:
		MeshManager_OpenGL( RetroResource::MeshCollection & meshes )
			: _meshes( meshes )
		{
		}

		virtual ~MeshManager_OpenGL() override
		{
			// TODO Make sure all meshes are unloaded from gpu.
		}

		virtual void load( RetroResource::Handle* handles, const u32 size = 1 ) override
		{
			// TODO meshes can either be regular, indexed, or tri strips!

			for ( u32 j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				u32 i = _meshes.handle_index.at( handle.id );
				const RetroResource::Mesh mesh = _meshes.mesh[ i ];

				Mesh gfx_mesh { 0, 0 };

				glGenBuffers( 1, &gfx_mesh.vbo );
				glBindBuffer( GL_ARRAY_BUFFER, gfx_mesh.vbo );
				glBufferData( GL_ARRAY_BUFFER, mesh.vertices_n*sizeof(GLfloat), mesh.vertices, GL_STATIC_DRAW );

				if ( mesh.indices != nullptr )
				{
					glGenBuffers( 1, &gfx_mesh.ibo );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gfx_mesh.ibo );
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, mesh.indices_n*sizeof(GLuint), mesh.indices, GL_STATIC_DRAW );
				}

				_handles.insert( { handle.id, gfx_mesh } );
			}
		}

		virtual void unload( RetroResource::Handle * handles, const u32 size = 1 ) override
		{
			for ( u32 j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];

				Mesh mesh = _handles.at( handle.id );
				glDeleteBuffers( 1, &mesh.vbo );

				if ( mesh.ibo != 0 )
				{
					glDeleteBuffers( 1, &mesh.ibo );
				}

				_handles.erase( handle.id );
			}
		}

		inline Mesh lookup( const RetroResource::Handle handle ) const
		{
			return _handles.at( handle.id );
		}
	};
}

#endif //A_RETRO_UI_GRAPHICS_MESHMANAGER_OPENGL_H
