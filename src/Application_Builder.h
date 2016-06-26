#ifndef A_RETRO_UI_APPLICATION_BUILDER_H
#define A_RETRO_UI_APPLICATION_BUILDER_H

#include <memory>
#include <vector>

#include "Ioc_Container.h"
#include "Application.h"

#include "Environment_Manager.h"

#include "Resource_HandleManager.h"

#include "Resource_MeshCollection.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"
#include "Resource_PackageCollection.h"

#include "Resource_MeshLoader.h"
#include "Resource_ShaderLoader.h"
#include "Resource_BitmapLoader.h"

#include "Resource_PackageLoader_Lua.h"

#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Graphics_MeshManager_OpenGL.h"

#include "Ecs_EntityManager.h"
#include "Ecs_Time.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

namespace RetroApplication
{
	class Builder
	{
		shared_ptr< Ioc::Container >            _ioc;
		vector< shared_ptr<RetroEcs::ISystem> > _systems;

	public:
		Builder()
			: _ioc( make_shared< Ioc::Container >() )
		{
			auto& ioc = *_ioc;

			// SELF :: So meta.

			ioc.instance( _ioc );

			ioc.singleton< RetroApplication::StayAlive >(); // TODO Replace with a command or message!

			// RETRO RESOURCES

			ioc.singleton< RetroResource::HandleManager >();
			ioc.singleton< RetroResource::MeshCollection >();
			ioc.singleton< RetroResource::BitmapCollection >();
			ioc.singleton< RetroResource::ShaderCollection >();
			ioc.singleton< RetroResource::PackageCollection >();

			ioc.singleton< RetroResource::MeshLoader >(
				*ioc.resolve< RetroResource::HandleManager >(),
				*ioc.resolve< RetroResource::MeshCollection >() );

			ioc.singleton< RetroResource::ShaderLoader >(
				*ioc.resolve< RetroResource::HandleManager >(),
				*ioc.resolve< RetroResource::ShaderCollection >() );

			ioc.singleton< RetroResource::BitmapLoader >(
				*ioc.resolve< RetroResource::HandleManager >(),
				*ioc.resolve< RetroResource::BitmapCollection >() );

			ioc.singleton< RetroResource::IPackageLoader, RetroResource::PackageLoader_Lua >(
				*ioc.resolve< RetroResource::HandleManager >(),
				*ioc.resolve< RetroResource::PackageCollection >(),
				*ioc.resolve< RetroResource::MeshCollection >(),
				*ioc.resolve< RetroResource::BitmapCollection >(),
				*ioc.resolve< RetroResource::ShaderCollection >() );

			// RETRO GRAPHICS :: REQUIRES RESOURCES

			ioc.singleton< RetroGraphics::IMeshManager, RetroGraphics::MeshManager_OpenGL >(
				*ioc.resolve< RetroResource::MeshCollection >() );

			ioc.singleton< RetroGraphics::ITextureManager, RetroGraphics::TextureManager_OpenGL >(
				*ioc.resolve< RetroResource::BitmapCollection >() );

			ioc.singleton< RetroGraphics::IShaderManager, RetroGraphics::ShaderManager_OpenGL >(
				*ioc.resolve< RetroResource::ShaderCollection >() );

			// RETRO ECS :: REQUIRES ???

			ioc.singleton< RetroEcs::EntityManager >();
			ioc.singleton< RetroEcs::TimeRaw >();
			ioc.singleton< RetroEcs::Time, RetroEcs::TimeRaw >();
		}

		inline void env( shared_ptr< RetroEnvironment::IManager > env )
		{
			_ioc->instance( env );
		}

		template< class T, typename ...Ts >
		inline void add()
		{
			_ioc->singleton< T, Ts... >();
		}

		template< class T, class ...Ts >
		inline void add( Ts&& ...ts )
		{
			_ioc->singleton< T, Ts... >( std::forward< Ts >( ts )... );
		}

		template< class I, class T, class ...Ts >
		inline void add( Ts&& ...ts )
		{
			_ioc->singleton< I, T, Ts... >( std::forward< Ts >( ts )... );
		}

		template< class T, typename ...Ts >
		void system()
		{
			_ioc->singleton< T, Ts... >();
			_systems.push_back( _ioc->resolve< T >() );
		}

		shared_ptr< RetroApplication::Application > build()
		{
			auto env = _ioc->resolve< RetroEnvironment::IManager >();

			if ( !env )
			{
				std::cout << "[Application::Builder::build] couldn't resolve environment manager." << std::endl;
				return nullptr;
			}

			auto stay_alive = _ioc->resolve< StayAlive >();

			return make_shared< RetroApplication::Application >( env, stay_alive, _systems );
		}
	};
}

#endif //A_RETRO_UI_APPLICATION_BUILDER_H
