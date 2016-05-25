#ifndef A_RETRO_UI_RESOURCE_SHADERLOADER_H
#define A_RETRO_UI_RESOURCE_SHADERLOADER_H

#include "platform.h"
#include "Resource_Handle.h"
#include "Resource_HandleManager.h"
#include "Resource_ShaderCollection.h"

namespace RetroResource
{
	class ShaderLoader
	{
		HandleManager    & _handle_manager;
		ShaderCollection & _shaders;

	public:
		ShaderLoader( HandleManager & handle_manager, ShaderCollection & shaders );
		u32 load( const char ** names, const char ** paths, Handle * handles, const u32 size = 1 );
		void unload( Handle * handles, const u32 size = 1 );
	};
}

#endif //A_RETRO_UI_RESOURCE_SHADERLOADER_H
