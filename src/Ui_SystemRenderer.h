#ifndef A_RETRO_UI_UI_SYSTEM_RENDERER_H
#define A_RETRO_UI_UI_SYSTEM_RENDERER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <memory>

#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::perspective
#include <glm/gtx/transform.hpp>        // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

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
			glDisable( GL_DEPTH_TEST );

			// TODO Only use depth for rendering solid objects, which we don't do...yet.
			//glEnable( GL_DEPTH_TEST );
			//glDepthFunc( GL_LEQUAL );
			//glDepthMask( GL_TRUE );

			glClearColor( 1.0f, 0.1f, 1.0f, 1.0f );
		}

		virtual void tick() override
		{
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glm::vec2 resolution = _env->resolution();
			float h = .5f * 1080.f;
			float w = h * resolution.x / resolution.y;
			float z_near = .1f;
			float z_far = 100.f;
			glm::mat4 proj = glm::ortho( -w, w, -h, h, z_near, z_far );
			//glm::mat4 proj = glm::perspective( glm::radians(90.f), ar, z_near, z_far );
			glm::mat4 view = glm::translate( glm::mat4(1.f), glm::vec3(0.f, 0.f, -2.f) );
			glm::mat4 vp = proj * view;

			for ( u32 i = 0, n = _c_render->size(); i < n; ++i )
			{
				Entity e = _c_render->_data.entity[ i ];
				RenderKey key = _c_render->_data.key[ i ];
				RenderCommand* _command = &_c_render->_data.data[ i ];

				// Build mvp (model view projection) matrix.
				// Build model matrix from srt (scale rotation translation).

				glm::mat4& mvp = reinterpret_cast< glm::mat4& >( _command->Common.mvp );

				glm::mat4 scale = glm::scale( glm::vec3(_c_transform->scale(e)) );
				glm::mat4 rotation = glm::rotate( 0.f, glm::vec3(0,0,1) );
				glm::mat4 translation = glm::translate( glm::mat4(1.f), glm::vec3(_c_transform->x(e), _c_transform->y(e), _c_transform->z(e)) );
				mvp = vp * translation * rotation * scale;

				RenderCommand& command = _renderer.submit( key );
				command = *_command; // TODO We shouldn't have to copy every command every tick! Graphics::Renderer should have access to ComponentRender.
			}

			_renderer.render();
			_env->swap();
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_RENDERER_H
