#ifndef A_RETRO_UI_UI_ENTITY_FACTORY_H
#define A_RETRO_UI_UI_ENTITY_FACTORY_H

#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

#include "Resource_MeshLoader.h"
#include "Resource_BitmapCollection.h"

#include "Graphics_ShaderManager.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Graphics_TextureManager.h"
#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_MeshManager.h"
#include "Graphics_MeshManager_OpenGL.h"
#include "Graphics_RenderKey.h"

#include "Ecs_EntityManager.h"

#include "Ui_RenderFuncs.h"
#include "Ui_ComponentGrid.h"
#include "Ui_ComponentTransform.h"
#include "Ui_ComponentRender.h"

namespace RetroUi
{
	using std::string;
	using std::shared_ptr;

	class EntityFactory
	{
		shared_ptr< RetroResource::BitmapCollection > _bitmaps;
		shared_ptr< RetroResource::MeshLoader >       _mesh_loader;
		shared_ptr< RetroEcs::EntityManager >         _entity_manager;
		shared_ptr< RetroGraphics::IShaderManager >   _shader_manager;
		shared_ptr< RetroGraphics::ITextureManager >  _texture_manager;
		shared_ptr< RetroGraphics::IMeshManager >     _mesh_manager;
		shared_ptr< RetroUi::RenderFuncs >            _render_funcs;
		shared_ptr< RetroUi::ComponentGrid >          _c_grid;
		shared_ptr< RetroUi::ComponentTransform >     _c_transform;
		shared_ptr< RetroUi::ComponentRender >        _c_render;

		RetroResource::Handle _quad_handle;

	public:
		EntityFactory(
			shared_ptr< RetroResource::BitmapCollection > bitmaps,
			shared_ptr< RetroResource::MeshLoader > mesh_loader,
			shared_ptr< RetroEcs::EntityManager > entity_manager,
			shared_ptr< RetroGraphics::IShaderManager > shader_manager,
			shared_ptr< RetroGraphics::ITextureManager > texture_manager,
			shared_ptr< RetroGraphics::IMeshManager > mesh_manager,
			shared_ptr< RetroUi::RenderFuncs > render_funcs,
			shared_ptr< RetroUi::ComponentGrid > c_grid,
			shared_ptr< RetroUi::ComponentTransform > c_transform,
			shared_ptr< RetroUi::ComponentRender > c_render )
			: _entity_manager( entity_manager )
			, _shader_manager( shader_manager )
			, _texture_manager( texture_manager )
			, _mesh_manager( mesh_manager )
			, _mesh_loader( mesh_loader )
			, _bitmaps( bitmaps )
			, _render_funcs( render_funcs )
			, _c_grid( c_grid )
			, _c_transform( c_transform )
			, _c_render( c_render )
		{
			VertexBitmap quad[ 4 ] = {
				-.5f, -.5f, .0f, 0.f, 0.f,
				-.5f,  .5f, .0f, 0.f, 1.f,
				 .5f, -.5f, .0f, 1.f, 0.f,
				 .5f,  .5f, .0f, 1.f, 1.f,
			};

			_quad_handle = _mesh_loader->load_single( "ui_entity_quad", quad, sizeof(quad), nullptr, 0 );
			auto& mm = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager );
			mm.load( &_quad_handle );
		}

		virtual ~EntityFactory()
		{
			auto& mm = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager );
			mm.unload( &_quad_handle );
		}

		Entity create_scene()
		{
			Entity e = _entity_manager->create();
			u32 i = _c_transform->create( e );
			_c_transform->_data.pos[ i ] = glm::vec3(0);
			_c_transform->_data.scale[ i ] = 1.f;
			return e;
		}

		void destroy_scene( Entity e )
		{
			_c_transform->destroy( e );
			_entity_manager->destroy( e );
		}

		Entity create_anchor( const Entity e_parent, const glm::vec3& pos )
		{
			Entity e = _entity_manager->create();

			u32 i = _c_transform->create( e );
			_c_transform->_data.pos[ i ] = pos;
			_c_transform->_data.scale[ i ] =  1.f;

			_c_transform->append_child( e_parent, e );

			return e;
		}

		void destroy_anchor( Entity e )
		{
			_c_transform->destroy( e );
			_entity_manager->destroy( e );
		}

		Entity create_grid( const Entity e_parent, const glm::vec3& pos, const glm::ivec2& grid_size, const glm::ivec2& cell_size )
		{
			Entity e = _entity_manager->create();
			u32 i;

			i = _c_transform->create( e );
			_c_transform->_data.pos[ i ] = pos;
			_c_transform->_data.scale[ i ] = 1.f;

			i = _c_grid->create( e );
			_c_grid->_data.grid_size[ i ] = grid_size;
			_c_grid->_data.cell_size[ i ] = cell_size;

			_c_transform->append_child( e_parent, e );

			return e;
		}

		void destroy_grid( Entity e )
		{
			_c_transform->destroy( e );
			_c_grid->destroy( e );
			_entity_manager->destroy( e );
		}

		Entity create_bitmap( const Entity e_parent, const string& shader, const string& bitmap, const glm::vec3& pos )
		{
			RetroGraphics::RenderKey key;
			key.RenderTranslucent.translucency_type = 1;
			key.RenderTranslucent.depth = static_cast< u32 >( pos.z );

			RetroGraphics::RenderCommand data;
			data.DrawBitmap.func = _render_funcs->draw_bitmap;
			data.DrawBitmap.vbo_n = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager ).lookup( _quad_handle ).vbo_n;
			data.DrawBitmap.vbo = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager ).lookup( _quad_handle ).vbo;
			data.DrawBitmap.shader = reinterpret_cast< RetroGraphics::ShaderManager_OpenGL& >( *_shader_manager ).lookup( shader );
			data.DrawBitmap.bitmap = reinterpret_cast< RetroGraphics::TextureManager_OpenGL& >( *_texture_manager ).lookup( bitmap );

			glm::vec2& size = reinterpret_cast< glm::vec2& >( data.DrawBitmap.size );
			size = _bitmaps->size[ _bitmaps->name_index.at(bitmap) ];

			Entity e = _entity_manager->create();
			u32 i;

			i = _c_transform->create( e );
			_c_transform->_data.pos[ i ] = pos;
			_c_transform->_data.scale[ i ] = 1.f;

			i = _c_render->create( e );
			_c_render->_data.key[ i ] = key;
			_c_render->_data.data[ i ] = data;

			_c_transform->append_child( e_parent, e );

			return e;
		}

		void destroy_bitmap( Entity e )
		{
			_c_transform->destroy( e );
			_c_render->destroy( e );
			_entity_manager->destroy( e );
		}

		Entity create_string( const Entity e_parent, const string& shader, const string& font_bitmap, const float font_size, const string& text, const glm::vec3& pos )
		{
			RetroGraphics::RenderKey key;
			key.RenderTranslucent.translucency_type = 1;
			key.RenderTranslucent.depth = static_cast< u32 >( pos.z );

			// TODO Create a mesh at some proper location!
			RetroResource::Handle str_handle = create_and_load_text_mesh( font_size, text );

			RetroGraphics::RenderCommand data;
			data.DrawString.func = _render_funcs->draw_string;
			data.DrawString.vbo_n = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager ).lookup( str_handle ).vbo_n;
			data.DrawString.vbo = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager ).lookup( str_handle ).vbo;
			data.DrawString.shader = reinterpret_cast< RetroGraphics::ShaderManager_OpenGL& >( *_shader_manager ).lookup( shader );
			data.DrawString.bitmap = reinterpret_cast< RetroGraphics::TextureManager_OpenGL& >( *_texture_manager ).lookup( font_bitmap );
			data.DrawString.font_size = font_size;
			data.DrawString.mesh_handle = str_handle;

			Entity e = _entity_manager->create();
			u32 i;

			i = _c_transform->create( e );
			_c_transform->_data.pos[ i ] = pos;
			_c_transform->_data.scale[ i ] = 1.f;

			i = _c_render->create( e );
			_c_render->_data.key[ i ] = key;
			_c_render->_data.data[ i ] = data;

			_c_transform->append_child( e_parent, e );

			return e;
		}

		void destroy_string( Entity e )
		{
			auto& mm = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager );
			RetroGraphics::RenderCommand& command = _c_render->data( e );
			mm.unload( &command.DrawString.mesh_handle );

			_c_transform->destroy( e );
			_c_render->destroy( e );
			_entity_manager->destroy( e );
		}

	private:
		u32 _string_hash = 0u;

		RetroResource::Handle create_and_load_text_mesh( const float font_size, const string& text )
		{
			++_string_hash;

			std::vector<VertexString> str;
			str.resize( text.size() );
			float char_width = 1.5f * font_size; // TODO Calc this in a proper way.
			float x = -0.5f * char_width * text.size();
			float y = .0f;
			u32 i = 0;

			for ( auto c : text )
			{
				str[ i++ ] = { (i32)c, x, y };
				x += char_width;
			}

			RetroResource::Handle handle = _mesh_loader->load_single( std::to_string(_string_hash), str.data(), sizeof(VertexString)*str.size(), nullptr, 0 );
			auto& mm = reinterpret_cast< RetroGraphics::MeshManager_OpenGL& >( *_mesh_manager );
			mm.load( &handle );

			return handle;
		}
	};
}

#endif //A_RETRO_UI_UI_ENTITY_FACTORY_H
