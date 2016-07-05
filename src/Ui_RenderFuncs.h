#ifndef A_RETRO_UI_UI_RENDERFUNCS_H
#define A_RETRO_UI_UI_RENDERFUNCS_H

#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

#include "Graphics_Renderer.h"
#include "Ui_ComponentRender.h"

namespace RetroUi
{
	class RenderFuncs
	{
	public:
		RetroGraphics::RenderFunc draw_bitmap;
		RetroGraphics::RenderFunc draw_string;
	};


	RenderFuncs render_funcs_OpenGL
	{
		.draw_bitmap = []( const RetroGraphics::RenderCommand* command )
		{
			glUseProgram( command->DrawBitmap.shader );

			glUniformMatrix4fv( glGetUniformLocation( command->DrawBitmap.shader, "mvp" ), 1, GL_FALSE, command->DrawBitmap.mvp );
			glUniform2fv( glGetUniformLocation( command->DrawBitmap.shader, "size" ), 1, command->DrawBitmap.size );

			int n_verts = 4;//sizeof(vertices) / sizeof(GL_FLOAT);
			glBindBuffer( GL_ARRAY_BUFFER, command->DrawBitmap.vbo );
			glVertexAttribPointer( glGetAttribLocation( command->DrawBitmap.shader, "pos" ), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), 0 );
			glVertexAttribPointer( glGetAttribLocation( command->DrawBitmap.shader, "uv" ), 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)) );
			//glVertexAttribPointer( glGetAttribLocation( data.shader, "col" ), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0 );

			// TODO bind relevant texture here.
			// TODO only activate and rebind if neccessary.
			int texture_i = 0;
			glActiveTexture( GL_TEXTURE0 + texture_i );
			glUniform1i( glGetUniformLocation( command->DrawBitmap.shader, "texture" ), texture_i );

			glBindTexture( GL_TEXTURE_2D, command->DrawBitmap.bitmap );
			glDrawArrays( GL_TRIANGLE_STRIP, 0, n_verts );
		},
	};
}

#endif //A_RETRO_UI_UI_RENDERFUNCS_H
