#ifndef A_RETRO_UI_RENDERER_H
#define A_RETRO_UI_RENDERER_H

#include <algorithm>

#include "platform.h"
#include "Resource_Handle.h"
#include "Graphics_RenderKey.h"
#include "Graphics_ShaderManager.h"
#include "Graphics_TextureManager.h"

/* TODO
 * Add a factory for render keys?
 * include glm?
 * Before including glm lets make sure we can draw several tris!
 *
 */

namespace
{
	inline bool compare_render_key ( RetroGraphics::RenderKey a, RetroGraphics::RenderKey b )
	{
		return a.raw < b.raw;
	}
}

namespace RetroGraphics
{
	struct RenderMaterial
	{
		u32 shader_handle;

		//uniforms for shader:
		// u32 texture_handle;
		// glm::vec4 color;
	};

	typedef void* RenderData;
	typedef void ( *RenderFunc )( const RenderData );

	struct RenderCommand
	{
		RenderData data;
		RenderFunc func;
	};


	class Renderer
	{
		u32              _render_count;
		RenderKey        _render_key[ 1024 ];
		RenderCommand    _render_command[ 1024 ];

	public:
		explicit Renderer()
			: _render_count( 0 )
		{}

		RenderCommand & submit( const RenderKey key )
		{
			_render_key[ _render_count ] = key;
			_render_key[ _render_count ].RenderCommon.data_index = _render_count;
			RenderCommand & command = _render_command[ _render_count ];
			++_render_count;
			return command;
		}

		void render()
		{
			std::sort( _render_key, _render_key + _render_count, compare_render_key );
			for ( u32 i = 0; i < _render_count; ++i )
			{
				int command_i = _render_key[ i ].RenderCommon.data_index;
				RenderCommand & command = _render_command[ command_i ];
				command.func( command.data );
			}
			_render_count = 0;
		}
	};
}

#endif //A_RETRO_UI_RENDERER_H
