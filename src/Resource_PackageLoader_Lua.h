#ifndef A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H
#define A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H

#include "Resource_PackageLoader.h"
#include "Resource_HandleManager.h"

#include "Resource_PackageCollection.h"
#include "Resource_FontCollection.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"

namespace RetroResource
{
	class PackageLoader_Lua : public IPackageLoader
	{
		HandleManager     & _handle_manager;
		FontCollection    & _fonts;
		BitmapCollection  & _bitmaps;
		ShaderCollection  & _shaders;
		PackageCollection & _packages;

	public:
		PackageLoader_Lua( HandleManager & handle_manager, PackageCollection & packages,
			FontCollection & fonts, BitmapCollection & bitmaps, ShaderCollection & shaders );

		virtual ~PackageLoader_Lua() override;
		virtual u32 load( const std::string name, Handle & handle ) override;
		virtual u32 load( const std::string * names, Handle * handles, const u32 size = 1 ) override;
		virtual void unload( const Handle * handles, const u32 size = 1 ) override;
	};
}

#endif //A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H
