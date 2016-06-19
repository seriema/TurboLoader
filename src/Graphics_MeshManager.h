#ifndef A_RETRO_UI_GRAPHICS_MESHMANAGER_H
#define A_RETRO_UI_GRAPHICS_MESHMANAGER_H

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroGraphics
{
	class IMeshManager
	{
	public:
		virtual ~IMeshManager() {}
		virtual void load( RetroResource::Handle* handles, const u32 size = 1 ) = 0;
		virtual void unload( RetroResource::Handle* handles, const u32 size = 1 ) = 0;
	};
}

#endif //A_RETRO_UI_GRAPHICS_MESHMANAGER_H
