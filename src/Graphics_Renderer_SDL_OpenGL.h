#ifndef A_RETRO_UI_GRAPHICS_RENDERER_SDL_OPENGL_H
#define A_RETRO_UI_GRAPHICS_RENDERER_SDL_OPENGL_H

#include <cstdint>
#include <algorithm>
#include <utility>
#include <iostream>
#include <sstream>

#include "platform.h"
#include "Graphics_Renderer.h"
#include "shader.h"
#include "Graphics_TextureManager_OpenGL.h"

namespace RetroGraphics
{
	class Renderer_SDL_OpenGL : public IRenderer
	{
		RenderMaterial    _render_material[ 1 ];

		RenderKey         _render_key[ RENDER_INPUT_ARRAY_SIZE ];
		RenderData        _render_data[ RENDER_INPUT_ARRAY_SIZE ];
		u32               _render_count;

		Shader*           _TEMP_shader;

		RetroResource::BitmapCollection * _bitmaps;
		TextureManager_OpenGL           * _texture_manager;

	public:
		Renderer_SDL_OpenGL( RetroResource::BitmapCollection * bitmaps, TextureManager_OpenGL * texture_manager );
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
