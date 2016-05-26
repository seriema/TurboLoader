#ifndef A_RETRO_UI_GUI_RENDERER_H
#define A_RETRO_UI_GUI_RENDERER_H

#include <utility>

#include "platform.h"
#include "Resource_Handle.h"
#include "Graphics_Renderer.h"

namespace RetroGui
{
	class Renderer
	{
		RetroGraphics::IRenderer & _renderer;

//		const float RECT[ 18 ] =
//		{
//				0.0f, 1.0f, 0.0f,
//				0.0f, 0.0f, 0.0f,
//				1.0f, 1.0f, 0.0f,
//
//				1.0f, 1.0f, 0.0f,
//				0.0f, 0.0f, 0.0f,
//				1.0f, 0.0f, 0.0f,
//		};

		const float RECT[ 4 * 3 ] =
		{
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
		};

		const u32 RECT_N = sizeof(RECT) / sizeof(float);

		u32 _vbo_handle;

	public:
		Renderer( RetroGraphics::IRenderer & renderer )
			: _renderer( renderer )
		{
			_vbo_handle = _renderer.add_mesh( RECT, RECT_N );
		}

		virtual ~Renderer() {}

		void draw_rect( RetroResource::Handle bitmap, RetroResource::Handle shader, vec2 pos, vec2 size )
		{
			// TODO Replace bitmap and shader with a single material.

			RetroGraphics::RenderKey render_key;
			RetroGraphics::RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo = _vbo_handle;
			render_data.bitmap = bitmap;
			render_data.shader = shader;
			render_data.pos = pos;

			_renderer.draw( render_key, render_data );
		}
	};
}

#endif //A_RETRO_UI_GUI_RENDERER_H
