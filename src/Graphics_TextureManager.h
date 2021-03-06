#ifndef A_RETRO_UI_GRAPHICS_TEXTUREMANAGER_H
#define A_RETRO_UI_GRAPHICS_TEXTUREMANAGER_H

#include "Resource_Handle.h"

namespace RetroGraphics
{
	class ITextureManager
	{
	public:
		virtual ~ITextureManager() {}
		virtual void load( RetroResource::Handle * handles, const u32 size = 1 ) = 0;
		virtual void unload( RetroResource::Handle * handles, const u32 size = 1 ) = 0;
		//virtual void bind( const RetroResource::Handle handle ) = 0;
	};
}

#endif //A_RETRO_UI_GRAPHICS_TEXTUREMANAGER_H
