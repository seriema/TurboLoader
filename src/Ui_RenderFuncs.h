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


	struct VertexBitmap
	{
		float pos[ 3 ];
		float uv[ 2 ];
	};


	struct VertexString
	{
		i32 char_index;
		float x;
		float y;
	};


	RenderFuncs render_funcs_OpenGL
	{
		// draw_bitmap
		[]( const RetroGraphics::RenderCommand* command )
		{
			glUseProgram( command->DrawBitmap.shader );

			glUniformMatrix4fv( glGetUniformLocation( command->DrawBitmap.shader, "mvp" ), 1, GL_FALSE, command->DrawBitmap.mvp );
			glUniform2fv( glGetUniformLocation( command->DrawBitmap.shader, "size" ), 1, command->DrawBitmap.size );

			int n_verts = command->DrawBitmap.vbo_n / sizeof( VertexBitmap );
			GLsizei stride = sizeof( VertexBitmap );
			auto uv_offset = (const void*)sizeof( VertexBitmap::pos );

			glBindBuffer( GL_ARRAY_BUFFER, command->DrawBitmap.vbo );
			glVertexAttribPointer( glGetAttribLocation( command->DrawBitmap.shader, "pos" ), 3, GL_FLOAT, GL_FALSE, stride, 0 );
			glVertexAttribPointer( glGetAttribLocation( command->DrawBitmap.shader, "uv" ), 2, GL_FLOAT, GL_FALSE, stride, uv_offset );
			//glVertexAttribPointer( glGetAttribLocation( data.shader, "col" ), 3, GL_FLOAT, GL_FALSE, stride, 0 );

			// TODO bind relevant texture here.
			// TODO only activate and rebind if neccessary.
			int texture_i = 0;
			glActiveTexture( GL_TEXTURE0 + texture_i );
			glUniform1i( glGetUniformLocation( command->DrawBitmap.shader, "texture" ), texture_i );

			glBindTexture( GL_TEXTURE_2D, command->DrawBitmap.bitmap );
			glDrawArrays( GL_TRIANGLE_STRIP, 0, n_verts );
		},

		// draw_string
		[]( const RetroGraphics::RenderCommand* command )
		{
			glUseProgram( command->DrawString.shader );

			float object_scale_y = command->DrawString.mvp[ 5 ];
			float resolution_scale = 480.f; // TODO Get this from command some how!
			glPointSize( object_scale_y * resolution_scale * command->DrawString.font_size );


//			glUniform1f( glGetUniformLocation( command->DrawString.shader, "timestamp" ), 0.001f * (float)SDL_GetTicks() );
			glUniformMatrix4fv( glGetUniformLocation( command->DrawString.shader, "mvp" ), 1, GL_FALSE, command->DrawString.mvp );

			u32 n_verts = command->DrawString.vbo_n / sizeof( VertexString );
			GLsizei stride = sizeof( VertexString );
			auto pos_offset = (const void*)sizeof( VertexString::char_index );

			glBindBuffer( GL_ARRAY_BUFFER, command->DrawString.vbo );
			glVertexAttribPointer( glGetAttribLocation( command->DrawString.shader, "char_index" ), 1, GL_INT, GL_FALSE, stride, 0 );
			glVertexAttribPointer( glGetAttribLocation( command->DrawString.shader, "pos" ), 2, GL_FLOAT, GL_FALSE, stride, pos_offset );
			//glVertexAttribPointer( glGetAttribLocation( data.shader, "col" ), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0 );

			// TODO bind relevant texture here.
			// TODO only activate and rebind if neccessary.
			int texture_i = 0;
			glActiveTexture( GL_TEXTURE0 + texture_i );
			glUniform1i( glGetUniformLocation( command->DrawString.shader, "texture" ), texture_i );

			glBindTexture( GL_TEXTURE_2D, command->DrawString.bitmap );
			glDrawArrays( GL_POINTS, 0, n_verts );
		},
	};
}

#endif //A_RETRO_UI_UI_RENDERFUNCS_H
