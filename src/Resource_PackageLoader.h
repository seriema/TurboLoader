#ifndef A_RETRO_UI_RESOURCE_PACKAGELOADER_H
#define A_RETRO_UI_RESOURCE_PACKAGELOADER_H

#include "platform.h"
#include "Resource_HandleManager.h"

namespace RetroResource
{
	class IPackageLoader
	{
	public:
		virtual ~IPackageLoader() {}
		virtual u32 load( const std::string name, Handle & handle ) = 0;
		virtual u32 load( const std::string * names, Handle * handles, const u32 size = 1 ) = 0;
		virtual void unload( const Handle * handles, const u32 size = 1 ) = 0;
	};
}

#endif //A_RETRO_UI_RESOURCE_PACKAGELOADER_H
