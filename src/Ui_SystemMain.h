#ifndef A_RETRO_UI_UI_SYSTEM_MAIN_H
#define A_RETRO_UI_UI_SYSTEM_MAIN_H

#include <memory>
#include <cmath>

#include "platform.h"

#include "Resource_Handle.h"
#include "Resource_PackageLoader.h"
#include "Resource_PackageCollection.h"

#include "Graphics_ShaderManager.h"
#include "Graphics_TextureManager.h"

#include "Ecs_System.h"

#include "Ui_EntityFactory.h"
#include "Ui_ComponentTransform.h"
#include "Ui_ComponentRender.h"

#include "Math.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemMain : public RetroEcs::ISystem
	{
		shared_ptr< RetroResource::IPackageLoader >    _package_loader;
		shared_ptr< RetroResource::PackageCollection > _packages;
		shared_ptr< RetroGraphics::IShaderManager >    _shader_manager;
		shared_ptr< RetroGraphics::ITextureManager >   _texture_manager;
		shared_ptr< Context >                          _ui;
		shared_ptr< EntityFactory >                    _entity_factory;
		shared_ptr< ComponentTransform >               _c_transform;

		Entity                   _main;
		RetroResource::Handle    _base_package_handle;

	public:
		explicit SystemMain(
			shared_ptr< RetroResource::IPackageLoader > package_loader,
			shared_ptr< RetroResource::PackageCollection > packages,
			shared_ptr< RetroGraphics::IShaderManager > shader_manager,
			shared_ptr< RetroGraphics::ITextureManager > texture_manager,
			shared_ptr< Context > ui,
			shared_ptr< EntityFactory > entity_factory,
			shared_ptr< ComponentTransform > c_transform )
			: _package_loader( package_loader )
			, _packages( packages )
			, _shader_manager( shader_manager )
			, _texture_manager( texture_manager )
			, _ui( ui )
			, _entity_factory( entity_factory )
			, _c_transform( c_transform )
		{
			_package_loader->load( "./src/hello_world", _base_package_handle );
			{
				auto & package = _packages->handle_lookup.at( _base_package_handle.id );
				_texture_manager->load( package.bitmaps.data(), package.bitmaps.size() );
				_shader_manager->load( package.shaders.data(), package.shaders.size() );
			}

			_ui->scene = _entity_factory->create_scene();
			_ui->focus = RetroEcs::Entity { 0 };

			_main = _entity_factory->create_grid( _ui->scene, glm::vec3(-800.f, 400.f, 0.f), glm::ivec2(10), glm::ivec2(166) );

			for ( u32 i = 0, n = 60; i < n; ++i )
			{
				std::string image = RetroMath::fnv1a( 99877*i ) < 2200000000u ? "jp" : "jb";
				_entity_factory->create_image( _main, "debug", image, glm::vec3(0) );
			}

			_ui->focus = _c_transform->_data.entity[ _c_transform->first_child(_main) ];
		}

		virtual ~SystemMain() override
		{
			// TODO When destroying then do iterate grid and delete images before deleting grid!
			// TODO Can we do some more general destroying of entity children via transforms?

//			for ( auto e : _entities )
//			{
//				_entity_factory->destroy_image( e );
//			}

			_package_loader->unload( &_base_package_handle );
		}

		virtual void tick() override
		{
			// TODO this should be triggered via an animation in animation system. But that remains to be built!

			for ( u32 i = 0, n = _c_transform->size(); i < n; ++i )
			{
				_c_transform->_data.scale[ i ] = _c_transform->_data.entity[ i ].id == _ui->focus.id ? 1.5f : 1.f;
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_MAIN_H
