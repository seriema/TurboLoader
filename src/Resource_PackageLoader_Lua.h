#ifndef A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H
#define A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H

#include "Resource_PackageLoader.h"
#include "Resource_MeshLoader.h"
#include "Resource_BitmapLoader.h"
#include "Resource_ShaderLoader.h"
#include "Resource_GuiViewCollection.h"

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
		GuiViewCollection& _views;

		MeshLoader         _mesh_loader;
		BitmapLoader       _bitmap_loader;
		ShaderLoader       _shader_loader;

		void load_gui_views( pt::ptree* views, Handle* handles, const u32 size );

	public:
		PackageLoader_Lua(
				HandleManager& handle_manager,
				PackageCollection& packages,
				MeshCollection& meshes,
				BitmapCollection& bitmaps,
				ShaderCollection& shaders,
				GuiViewCollection& views );

		virtual ~PackageLoader_Lua() override;
		virtual u32 load( const std::string name, Handle& handle ) override;
		virtual u32 load( const std::string* names, Handle* handles, const u32 size = 1 ) override;
		virtual void unload( const Handle* handles, const u32 size = 1 ) override;
	};
}

#endif //A_RETRO_UI_RESOURCE_PACKAGELOADER_LUA_H
