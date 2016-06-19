#ifndef A_RETRO_UI_UI_SYSTEM_RENDERER_H
#define A_RETRO_UI_UI_SYSTEM_RENDERER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <memory>

#include "platform.h"

#include "Ecs_EntityManager.h"
#include "Ecs_System.h"

#include "Ui_ComponentRender.h"
#include "Ui_ComponentTransform.h"

#include "Resource_Handle.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"

#include "Graphics_Renderer.h"
#include "Graphics_RenderKey.h"

namespace
{
	using RetroGraphics::RenderKey;
	using RetroGraphics::RenderData;
	using RetroUi::RenderData_Draw;

	std::string calc_description()
	{
		std::ostringstream description;
		description << "OpenGL env" << std::endl;
		description << "   Version: " << glGetString( GL_VERSION ) << std::endl;
		description << "    Vendor: " << glGetString( GL_VENDOR ) << std::endl;
		description << "  Renderer: " << glGetString( GL_RENDERER ) << std::endl;
		description << "   Shading: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
		return description.str();
	}

	void Draw_OpenGL( const RenderData _data )
	{
		auto& data = *reinterpret_cast< const RenderData_Draw* >( _data );

		glUseProgram( data.shader );

		glUniform2fv( glGetUniformLocation( data.shader, "model_pos" ), 1, data.pos.raw );

		int n_verts = 4;//sizeof(vertices) / sizeof(GL_FLOAT);
		glBindBuffer( GL_ARRAY_BUFFER, data.vbo );
		glVertexAttribPointer( glGetAttribLocation( data.shader, "vert" ), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0 );

		// TODO bind relevant texture here.
		// TODO only activate and rebind if neccessary.
		int texture_i = 0;
		glActiveTexture( GL_TEXTURE0 + texture_i );
		glUniform1i( glGetUniformLocation( data.shader, "texture" ), texture_i );

		glBindTexture( GL_TEXTURE_2D, data.bitmap );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, n_verts );
	};
}

namespace RetroUi
{
	using std::vector;
	using std::shared_ptr;

	using RetroGraphics::RenderKey;
	using RetroGraphics::RenderFunc;
	using RetroGraphics::RenderCommand;
	using RetroGraphics::Renderer;

	class SystemRenderer : public RetroEcs::ISystem
	{
		shared_ptr< RetroEnvironment::IManager > _env;
		shared_ptr< ComponentTransform > _c_transform;
		shared_ptr< ComponentRender >    _c_render;
		Renderer                         _renderer;

	public:
		explicit SystemRenderer(
				shared_ptr< RetroEnvironment::IManager > env,
				shared_ptr< ComponentTransform > c_transform,
				shared_ptr< ComponentRender > c_render )
			: _env( env )
			, _c_render( c_render )
			, _c_transform( c_transform )
		{
			std::cout << calc_description() << std::endl;

			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );
			glFrontFace( GL_CW );

			// fragment blending
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			// depth
			glEnable( GL_DEPTH_TEST );
			glDepthFunc( GL_LEQUAL );
			glDepthMask( GL_TRUE );

			glClearColor( 1.0f, 0.1f, 1.0f, 1.0f );
		}

		virtual void tick() override
		{
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			for ( u32 i = 0, n = _c_render->size(); i < n; ++i )
			{
				Entity e = _c_render->_data.entity[ i ];
				RenderKey key = _c_render->_data.key[ i ];
				RenderData_Draw* data = &_c_render->_data.data[ i ];
				data->pos.x = _c_transform->x( e );
				data->pos.y = _c_transform->y( e );

				RenderCommand& command = _renderer.submit( key );
				command.data = data;
				command.func = Draw_OpenGL;
			}

			_renderer.render();
			_env->swap();
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_RENDERER_H
