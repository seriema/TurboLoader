#ifndef A_RETRO_UI_GRAPHICS_RENDERER_SDL_OPENGL_H
#define A_RETRO_UI_GRAPHICS_RENDERER_SDL_OPENGL_H

#include "platform.h"
#include "Graphics_Renderer.h"

class Shader;

namespace RetroResource
{
	class BitmapCollection;
	class ShaderCollection;
}

namespace RetroGraphics
{
	class TextureManager_OpenGL;
	class ShaderManager_OpenGL;
}

namespace RetroGraphics
{
	class Renderer_SDL_OpenGL : public IRenderer
	{
		RenderMaterial    _render_material[ 1 ];

		RenderKey         _render_key[ RENDER_INPUT_ARRAY_SIZE ];
		RenderData        _render_data[ RENDER_INPUT_ARRAY_SIZE ];
		u32               _render_count;

		RetroResource::BitmapCollection * _bitmaps;
		RetroResource::ShaderCollection * _shaders;
		TextureManager_OpenGL           * _texture_manager;
		ShaderManager_OpenGL            * _shader_manager;

	public:
		explicit Renderer_SDL_OpenGL(
				RetroResource::BitmapCollection * bitmaps,
				RetroResource::ShaderCollection * shaders,
				TextureManager_OpenGL * texture_manager,
				ShaderManager_OpenGL * shader_manager );
		virtual ~Renderer_SDL_OpenGL() override;

		virtual std::string calc_description() const override;

		// Mesh

		virtual GLuint add_mesh( const GLfloat* vertices, int n_vertices ) override;
		virtual bool del_mesh( GLuint vbo_handle ) override;

		// Render

		virtual void draw( const RenderKey& render_key, const RenderData& render_data ) override;
		virtual void render() override;
	};
}

#endif //A_RETRO_UI_GRAPHICS_RENDERER_SDL_OPENGL_H
