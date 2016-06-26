#ifndef A_RETRO_UI_UI_ENTITY_FACTORY_H
#define A_RETRO_UI_UI_ENTITY_FACTORY_H

#include <iostream>

#include "Resource_MeshLoader.h"
#include "Resource_MeshCollection.h"

#include "Graphics_ShaderManager.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Graphics_TextureManager.h"
#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_MeshManager.h"
#include "Graphics_MeshManager_OpenGL.h"
#include "Graphics_RenderKey.h"

#include "Ecs_EntityManager.h"

#include "Ui_ComponentRender.h"
#include "Ui_ComponentTransform.h"

namespace RetroUi
{
	using RetroResource::MeshLoader;
	using RetroResource::MeshCollection;

	using RetroGraphics::IShaderManager;
	using RetroGraphics::ITextureManager;
	using RetroGraphics::IMeshManager;

	using RetroEcs::EntityManager;
	using RetroEcs::Entity;

	using std::string;
	using std::shared_ptr;

	class EntityFactory
	{
		shared_ptr< EntityManager >   _entity_manager;
		shared_ptr< IShaderManager >  _shader_manager;
		shared_ptr< ITextureManager > _texture_manager;
		shared_ptr< IMeshManager >    _mesh_manager;
		shared_ptr< MeshLoader >      _mesh_loader;
		shared_ptr< RetroUi::ComponentTransform > _c_transform;
		shared_ptr< RetroUi::ComponentRender >    _c_render;

		RetroResource::Handle _quad_handle;

	public:
		EntityFactory(
			shared_ptr< EntityManager > entity_manager,
			shared_ptr< IShaderManager > shader_manager,
			shared_ptr< ITextureManager > texture_manager,
			shared_ptr< IMeshManager > mesh_manager,
			shared_ptr< MeshLoader > mesh_loader,
			shared_ptr< RetroUi::ComponentTransform > c_transform,
			shared_ptr< RetroUi::ComponentRender > c_render )
			: _entity_manager( entity_manager )
			, _shader_manager( shader_manager )
			, _texture_manager( texture_manager )
			, _mesh_manager( mesh_manager )
			, _mesh_loader( mesh_loader )
			, _c_transform( c_transform )
			, _c_render( c_render )
		{
			float quad[ 4 * (3+2) ] = {
				-.3f, -.3f, .0f, 0.f, 0.f,
				-.3f,  .3f, .0f, 0.f, 1.f,
				 .3f, -.3f, .0f, 1.f, 0.f,
				 .3f,  .3f, .0f, 1.f, 1.f,
			};
			const u32 quad_n = sizeof( quad ) / sizeof( float );

			_quad_handle = _mesh_loader->load_single( "ui_entity_quad", quad, quad_n, nullptr, 0 );
			auto& mm = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager );
			mm.load( &_quad_handle );
		}

		virtual ~EntityFactory()
		{
			auto& mm = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager );
			mm.unload( &_quad_handle );
		}

		Entity create( const string& shader, const string& bitmap, float x, float y )
		{
			RetroGraphics::RenderKey key;
			key.RenderTranslucent.translucency_type = 1;
			key.RenderTranslucent.depth = 1;

			RetroUi::RenderData_Draw data;
			data.vbo = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager ).lookup( _quad_handle ).vbo;

			data.shader = reinterpret_cast< RetroGraphics::ShaderManager_OpenGL& >( *_shader_manager ).lookup( shader );
			data.bitmap = reinterpret_cast< RetroGraphics::TextureManager_OpenGL& >( *_texture_manager ).lookup( bitmap );

			Entity e = _entity_manager->create();

			_c_transform->create( e );
			_c_transform->set_x( e, x );
			_c_transform->set_y( e, y );
			_c_transform->set_z( e, 0.0f );

			_c_render->create( e );
			_c_render->set_key( e, key );
			_c_render->set_data( e, data );

			return e;
		}

		void destroy( Entity e )
		{
			_c_transform->destroy( e );
			_c_render->destroy( e );
		}
	};
}

#endif //A_RETRO_UI_UI_ENTITY_FACTORY_H
