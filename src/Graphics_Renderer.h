#ifndef A_RETRO_UI_RENDERER_H
#define A_RETRO_UI_RENDERER_H

#include "platform.h"
#include "Resource_Handle.h"
#include "Graphics_RenderKey.h"

/* TODO
 * Add a factory for render keys?
 * include glm?
 * Before including glm lets make sure we can draw several tris!
 *
 */

namespace RetroGraphics
{
	struct RendererSettings
	{
	};

	struct RenderMaterial
	{
		u32 shader_handle;

		//uniforms for shader:
		// u32 texture_handle;
		// glm::vec4 color;
	};

	struct RenderData
	{
		u32 vbo; // vbo contains attributes such as vert,norm,color, these must then also exist in shader.
		RetroResource::Handle shader;
		RetroResource::Handle bitmap;
		vec2 pos;
		//glm::mat4 model_transform;
	};

	namespace Aux
	{
		inline bool CompareRenderKey ( RenderKey a, RenderKey b )
		{
			return a.raw < b.raw;
		}
	}
}

namespace RetroGraphics
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual std::string calc_description() const = 0;
		virtual uint32_t add_mesh( const float *vertices, int n_vertices ) = 0;
		virtual bool del_mesh( uint32_t mesh_handle ) = 0;
		virtual void draw( const RenderKey& render_key, const RenderData& render_data ) = 0;
		virtual void render() = 0;

	};
}

#endif //A_RETRO_UI_RENDERER_H
