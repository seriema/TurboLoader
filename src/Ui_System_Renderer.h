#ifndef A_RETRO_UI_UI_SYSTEM_RENDERER_H
#define A_RETRO_UI_UI_SYSTEM_RENDERER_H

#include <vector>
#include <utility>

#include "platform.h"
#include "Ecs_EntityManager.h"
#include "Ecs_System.h"
#include "Ui_Component_Bitmap.h"
#include "Ui_Component_Transform.h"


#include "Resource_Handle.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"
#include "Graphics_Renderer.h"
#include "Gui_Renderer.h"

namespace RetroUi
{
	using std::vector;
	using std::pair;

	class SystemRenderer : public RetroEcs::System
	{
		ComponentBitmap         & _comp_bitmap;
		ComponentTransform      & _comp_transform;

		RetroGui::Renderer & _gui_renderer;

		//Renderer_Gui _renderer;

	public:
		explicit SystemRenderer(
				ComponentBitmap & comp_bitmap,
				ComponentTransform & comp_transform,
				RetroGui::Renderer & gui_renderer )
			: _comp_bitmap( comp_bitmap )
			, _comp_transform( comp_transform )
			, _gui_renderer( gui_renderer )
		{
		}

		virtual void tick( u32 dt ) override
		{
			for ( u32 i = 0, n = _comp_bitmap.size(); i < n; ++i )
			{
				Entity e = _comp_bitmap._data.entity[ i ];

				RetroResource::Handle bitmap_handle = _comp_bitmap._data.bitmap[ i ];
				RetroResource::Handle shader_handle = _comp_bitmap._data.shader[ i ];
				vec2 pos = { _comp_transform.x(e), _comp_transform.y(e) };
				vec2 size = { 1.0f, 1.0f };

				_gui_renderer.draw_rect( bitmap_handle, shader_handle, pos, size );
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_RENDERER_H
