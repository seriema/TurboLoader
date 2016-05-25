#ifndef A_RETRO_UI_RENDERER_H
#define A_RETRO_UI_RENDERER_H

#include "platform.h"

#define RENDER_INPUT_BITS_DATA_INDEX 13
#define RENDER_INPUT_BITS_DEPTH 8

#define RENDER_INPUT_ARRAY_SIZE 1 << RENDER_INPUT_BITS_DATA_INDEX

#define RENDER_INPUT_DATA_INDEX        u32 data_index        : RENDER_INPUT_BITS_DATA_INDEX;
#define RENDER_INPUT_MATERIAL_INDEX    u32 material_index    : 8;
#define RENDER_INPUT_DEPTH             u32 depth             : RENDER_INPUT_BITS_DEPTH;
#define RENDER_INPUT_IS_COMMAND        u32 is_command        : 1;
#define RENDER_INPUT_TRANSLUCENCY_TYPE u32 translucency_type : 2;



/* TODO
 * Add a factory for render keys?
 * include glm?
 * Before including glm lets make sure we can draw several tris!
 *
 */

namespace RetroGraphics
{
	union RenderKey
	{
		u32 raw;

		struct
		{
			RENDER_INPUT_DATA_INDEX
			u32 padding : 16;
			RENDER_INPUT_IS_COMMAND
			RENDER_INPUT_TRANSLUCENCY_TYPE
		} RenderCommon;

		struct
		{
			RENDER_INPUT_DATA_INDEX
			RENDER_INPUT_DEPTH
			RENDER_INPUT_MATERIAL_INDEX
			RENDER_INPUT_IS_COMMAND
			RENDER_INPUT_TRANSLUCENCY_TYPE
		} RenderOpaque;

		struct
		{
			RENDER_INPUT_DATA_INDEX
			RENDER_INPUT_MATERIAL_INDEX
			RENDER_INPUT_DEPTH
			RENDER_INPUT_IS_COMMAND
			RENDER_INPUT_TRANSLUCENCY_TYPE
		} RenderTranslucent;

		struct
		{
			RENDER_INPUT_DATA_INDEX
			u32 command : 10;
			u32 sequence : 6;
			RENDER_INPUT_IS_COMMAND
			RENDER_INPUT_TRANSLUCENCY_TYPE
		} RenderCommand;
	};
}

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
		u32 vbo_handle; // vbo contains attributes such as vert,norm,color, these must then also exist in shader.
		//glm::mat4 model_transform;

		union
		{
			float pos[2];
			struct { float x, y; };
		};
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
