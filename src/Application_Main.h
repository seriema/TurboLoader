#ifndef A_RETRO_UI_APPLICATION_MAIN_H
#define A_RETRO_UI_APPLICATION_MAIN_H

#include <iostream>
#include <vector>

#include "Application.h"
#include "EnvironmentManager.h"
#include "Graphics_Renderer.h"
#include "Gui_Renderer.h"
#include "Input.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"

class Application_Main : public IApplication
{
	IEnvironmentManager      * _environment_manager;
	RetroGraphics::IRenderer * _renderer;
	RetroGui::Renderer       * _gui_renderer;
	Input                    * _input;
	RetroResource::BitmapCollection & _bitmaps;
	RetroResource::ShaderCollection & _shaders;

	u32                        _dt;
	std::vector< std::pair<RetroGraphics::RenderKey, RetroGraphics::RenderData> > _objects;

public:
	Application_Main( IEnvironmentManager * environment_manager, RetroGraphics::IRenderer * renderer,
			RetroGui::Renderer * gui_renderer, Input * input,
			RetroResource::BitmapCollection & bitmaps, RetroResource::ShaderCollection & shaders )
		: _environment_manager( environment_manager )
		, _renderer( renderer )
		, _gui_renderer( gui_renderer )
		, _input( input )
		, _bitmaps( bitmaps )
		, _shaders( shaders )
	{
		GLenum error = glGetError();
		if( error != GL_NO_ERROR )
		{
			std::cout << "Unable to initialize OpenGL! Error: " << error << std::endl;
		}
		else
		{
			std::cout << _renderer->calc_description() << std::endl;
			init_scene();
		}
	}

	virtual ~Application_Main() override
	{
	}

	void init_scene()
	{
		GLfloat vertices[] =
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};
		int n_vertices = sizeof(vertices) / sizeof(GL_FLOAT);
		const GLuint vbo_handle = _renderer->add_mesh( vertices, n_vertices );

		RetroResource::Handle bitmap_handle_jp = _bitmaps.handle[ _bitmaps.name_index.at( "jp" ) ];
		RetroResource::Handle bitmap_handle_jb = _bitmaps.handle[ _bitmaps.name_index.at( "jb" ) ];
		RetroResource::Handle shader_handle = _shaders.handle[ _shaders.name_index.at( "debug" ) ];

		// Allocate triangle 1.
		{
			RetroGraphics::RenderKey render_key;
			RetroGraphics::RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo = vbo_handle;
			render_data.bitmap = bitmap_handle_jp;
			render_data.shader = shader_handle;
			render_data.pos.x = -0.5f;
			render_data.pos.y = -0.1f;

			_objects.push_back( { render_key, render_data } );
		}

		// Allocate triangle 2.
		{
			RetroGraphics::RenderKey render_key;
			RetroGraphics::RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo = vbo_handle;
			render_data.bitmap = bitmap_handle_jp;
			render_data.shader = shader_handle;
			render_data.pos.x = 0.3f;
			render_data.pos.y = 0.6f;

			_objects.push_back( { render_key, render_data } );
		}

		// Allocate triangle 3.
		{
			RetroGraphics::RenderKey render_key;
			RetroGraphics::RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo = vbo_handle;
			render_data.bitmap = bitmap_handle_jb;
			render_data.shader = shader_handle;
			render_data.pos.x = 0.9f;
			render_data.pos.y = -0.6f;

			_objects.push_back( { render_key, render_data } );
		}
	}

	virtual void loop() override
	{
		printf("Entering main loop\n");

		uint32_t t0 = SDL_GetTicks() - 17;

		while( !_input->quit_requested() )
		{
			uint32_t t1 = SDL_GetTicks();
			_dt = t1 - t0;
			t0 = t1;
			tick();
		}

		for ( auto obj : _objects )
		{
			_renderer->del_mesh( obj.second.vbo );
		}
	}

	virtual void tick() override
	{
		_input->poll_events();

		for ( auto& obj : _objects )
		{
			obj.second.pos.x += 0.0001f * _dt;
			if ( obj.second.pos.x > 1.1f )
				obj.second.pos.x = -1.1f;
		}

		for ( auto& obj : _objects )
		{
			_renderer->draw( obj.first, obj.second );
		}

		// TODO Proof of concept immediate gui draw example.
		{
			vec2 size = { 1.0f, 1.0f }; // TODO Not used yet.

			vec2 pos_jp = { -0.6f, 0.1f };
			vec2 pos_jb = { 0.1f, -0.35f };
			RetroResource::Handle bitmap_handle_jp = _bitmaps.handle[ _bitmaps.name_index[ "jp" ] ];
			RetroResource::Handle bitmap_handle_jb = _bitmaps.handle[ _bitmaps.name_index[ "jb" ] ];
			RetroResource::Handle shader_handle_debug = _shaders.handle[ _shaders.name_index[ "debug" ] ];
			_gui_renderer->draw_bitmap( bitmap_handle_jp, shader_handle_debug, pos_jp, size );
			_gui_renderer->draw_bitmap( bitmap_handle_jb, shader_handle_debug, pos_jb, size );

			vec2 pos_msdf = { 0.0f, 0.0f };
			RetroResource::Handle bitmap_handle_msdf = _bitmaps.handle[ _bitmaps.name_index[ "msdf" ] ];
			RetroResource::Handle shader_handle_msdf = _shaders.handle[ _shaders.name_index[ "msdf" ] ];
			_gui_renderer->draw_bitmap( bitmap_handle_msdf, shader_handle_msdf, pos_msdf, size );
		}

		_renderer->render();
		_environment_manager->swap();
	}

	virtual bool is_alive() override
	{
		return true;
	}
};

#endif //A_RETRO_UI_APPLICATION_MAIN_H
