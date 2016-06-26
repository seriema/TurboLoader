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

namespace RetroUi
{
	using std::shared_ptr;

	class SystemMain : public RetroEcs::ISystem
	{
		shared_ptr< RetroResource::IPackageLoader >    _package_loader;
		shared_ptr< RetroResource::PackageCollection > _packages;
		shared_ptr< RetroGraphics::IShaderManager >    _shader_manager;
		shared_ptr< RetroGraphics::ITextureManager >   _texture_manager;
		shared_ptr< EntityFactory >                    _entity_factory;

		std::vector< Entity >    _entities;
		RetroResource::Handle    _base_package_handle;

	public:
		explicit SystemMain(
			shared_ptr< RetroResource::IPackageLoader > package_loader,
			shared_ptr< RetroResource::PackageCollection > packages,
			shared_ptr< RetroGraphics::IShaderManager > shader_manager,
			shared_ptr< RetroGraphics::ITextureManager > texture_manager,
			shared_ptr< EntityFactory > entity_factory )
			: _package_loader( package_loader )
			, _packages( packages )
			, _shader_manager( shader_manager )
			, _texture_manager( texture_manager )
			, _entity_factory( entity_factory )
		{
			_package_loader->load( "./src/hello_world", _base_package_handle );
			{
				auto & package = _packages->handle_lookup.at( _base_package_handle.id );
				_texture_manager->load( package.bitmaps.data(), package.bitmaps.size() );
				_shader_manager->load( package.shaders.data(), package.shaders.size() );
			}

			_entities.push_back( _entity_factory->create("debug", "jp", glm::vec3(300.f, 400.f, .0f)) );
			_entities.push_back( _entity_factory->create("debug", "jp", glm::vec3(-400.f, -100.f, .0f)) );
			_entities.push_back( _entity_factory->create("debug", "jp", glm::vec3(800.f, -150.f, .0f)) );

			_entities.push_back( _entity_factory->create("debug", "jb", glm::vec3(-800.f, 100.f, .0f)) );
			_entities.push_back( _entity_factory->create("debug", "jb", glm::vec3(200.f, -300.f, .0f)) );
		}

		virtual ~SystemMain() override
		{
			for ( auto e : _entities )
			{
				_entity_factory->destroy( e );
			}

			_package_loader->unload( &_base_package_handle );
		}

		virtual void tick() override
		{
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_MAIN_H
