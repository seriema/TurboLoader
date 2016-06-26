#ifndef A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H
#define A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H

#include "Resource_PackageLoader.h"
#include "Resource_MeshLoader.h"
#include "Resource_BitmapLoader.h"
#include "Resource_ShaderLoader.h"

namespace RetroResource
{
	class HandleManager;
	struct PackageCollection;
}

namespace RetroResource
{
	class PackageLoader_Lua : public IPackageLoader
	{
		HandleManager&     _handle_manager;
		PackageCollection& _packages;
		MeshCollection&    _meshes;
		BitmapCollection&  _bitmaps;
		ShaderCollection&  _shaders;

		MeshLoader         _mesh_loader;
		BitmapLoader       _bitmap_loader;
		ShaderLoader       _shader_loader;

	public:
		PackageLoader_Lua(
			HandleManager& handle_manager,
			PackageCollection& packages,
			MeshCollection& meshes,
			BitmapCollection& bitmaps,
			ShaderCollection & shaders );

		virtual ~PackageLoader_Lua() override;
		virtual u32 load( const std::string name, Handle& handle ) override;
		virtual u32 load( const std::string* names, Handle* handles, const u32 size = 1 ) override;
		virtual void unload( const Handle* handles, const u32 size = 1 ) override;
	};
}

#endif //A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H
