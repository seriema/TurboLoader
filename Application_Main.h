#ifndef A_RETRO_UI_APPLICATION_MAIN_H
#define A_RETRO_UI_APPLICATION_MAIN_H

#include <iostream>
#include <vector>

#include "Application.h"
#include "EnvironmentManager.h"
#include "Renderer.h"


class Application_Main : public IApplication
{
public:
	Application_Main(
		const std::shared_ptr< IEnvironmentManager >& environment_manager,
		const std::shared_ptr< IRenderer >& renderer )
		: _environment_manager( environment_manager )
		, _renderer( renderer )
	{
		GLenum error = glGetError();
		if( error != GL_NO_ERROR )
		{
			std::cout << "Unable to initialize OpenGL! Error: " << error << std::endl;
		}
		else
		{
			std::cout << renderer->calc_description() << std::endl;
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
				0.0f,  0.2f, 0.0f,
				-0.15f, 0.0f, 0.0f,
				0.15f, 0.0f, 0.0f,
			};
		int n_vertices = sizeof(vertices) / sizeof(GL_FLOAT);
		const GLuint vbo_handle = _renderer->add_mesh( vertices, n_vertices );

		// Allocate triangle 1.
		{
			RenderKey render_key;
			RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo_handle = vbo_handle;
			render_data.x = -0.5f;
			render_data.y = -0.1f;

			_objects.push_back( { render_key, render_data } );
		}

		// Allocate triangle 2.
		{
			RenderKey render_key;
			RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo_handle = vbo_handle;
			render_data.x = 0.3f;
			render_data.y = 0.6f;

			_objects.push_back( { render_key, render_data } );
		}

		// Allocate triangle 3.
		{
			RenderKey render_key;
			RenderData render_data;

			render_key.RenderOpaque.material_index = 0;
			render_data.vbo_handle = vbo_handle;
			render_data.x = 0.9f;
			render_data.y = -0.6f;

			_objects.push_back( { render_key, render_data } );
		}

	}

	virtual void loop() override
	{
		SDL_Event e;
		SDL_StartTextInput();

		printf("Entering main loop\n");

		uint32_t t0 = SDL_GetTicks() - 17;
		bool is_looping = true;

		while( is_looping )
		{
			uint32_t t1 = SDL_GetTicks();
			_dt = t1 - t0;
			t0 = t1;

			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					is_looping = false;
				}
				//Handle keypress with current mouse position
				else if( e.type == SDL_TEXTINPUT )
				{
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );

					if ( e.text.text[ 0 ] == 'q' )
					{
						is_looping = false;
					}
				}
			}

			tick();
		}

		for ( auto obj : _objects )
		{
			_renderer->del_mesh( obj.second.vbo_handle );
		}

		//Disable text input
		SDL_StopTextInput();
	}

	virtual void tick() override
	{
		for ( auto& obj : _objects )
		{
			obj.second.x += 0.0001f * _dt;
			if ( obj.second.x > 1.1f )
				obj.second.x = -1.1f;
		}

		for ( auto& obj : _objects )
		{
			_renderer->draw( obj.first, obj.second );
		}

		_renderer->render();
		_environment_manager->swap();
	}

	virtual bool is_alive() override
	{
		return true;
	}

private:
	std::shared_ptr< IEnvironmentManager >           _environment_manager;
	std::shared_ptr< IRenderer >                     _renderer;

	std::vector< std::pair<RenderKey, RenderData> >  _objects;

	uint32_t _dt;
};

#endif //A_RETRO_UI_APPLICATION_MAIN_H
