#ifndef A_RETRO_UI_GRAPHICS_SHADERMANAGER_OPENGL_H
#define A_RETRO_UI_GRAPHICS_SHADERMANAGER_OPENGL_H

#include <unordered_map>
#include <queue>

// TODO Remove free image dependency from this file!
#include <FreeImage.h>

#include "platform.h"
#include "Graphics_ShaderManager.h"
#include "Resource_ShaderCollection.h"

namespace RetroGraphics
{
	class ShaderManager_OpenGL : public IShaderManager
	{
		RetroResource::ShaderCollection & _shaders;
		std::unordered_map< u32, u32 >    _handles;

	public:
		ShaderManager_OpenGL( RetroResource::ShaderCollection & shaders )
			: _shaders( shaders )
		{

		}

		virtual ~ShaderManager_OpenGL() override
		{
			// TODO make sure all shaders are unloaded.
		}

		virtual void load( RetroResource::Handle * handles, const u32 size = 1 ) override
		{
			for ( int j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				u32 i = _shaders.handle_index[ handle.id ];
				const char * vert = _shaders.vert[ i ];
				const char * frag = _shaders.frag[ i ];

				GLuint vert_handle;
				if ( !create_shader(vert, GL_VERTEX_SHADER, vert_handle ) )
					continue;

				GLuint frag_handle;
				if ( !create_shader(frag, GL_FRAGMENT_SHADER, frag_handle ) )
					continue;

				GLuint prog_handle;
				if ( !create_program( vert_handle, frag_handle, prog_handle ) )
					continue;

				glDeleteShader( vert_handle );
				glDeleteShader( frag_handle );

				_handles.insert( { handle.id, prog_handle } );
			}
		}

		virtual void unload( RetroResource::Handle * handles, const u32 size = 1 ) override
		{
			for ( int j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				glDeleteProgram( _handles.at( handle.id ) );
			}
		}

		void bind( const RetroResource::Handle handle ) const
		{
			u32 prog_handle = _handles.at( handle.id );
			glUseProgram( prog_handle );
		}

		u32 program( const RetroResource::Handle handle ) const
		{
			return _handles.at( handle.id );
		}

	private:
		bool create_shader( const char * shader_source, GLenum shader_type, GLuint & shader_handle ) const
		{
			shader_handle = glCreateShader( shader_type );
			glShaderSource( shader_handle, 1, &shader_source, NULL );
			glCompileShader( shader_handle );

			GLint success;
			glGetShaderiv( shader_handle, GL_COMPILE_STATUS, &success );

			if ( success != GL_TRUE )
			{
				GLsizei len;
				glGetShaderiv( shader_handle, GL_INFO_LOG_LENGTH, &len );
				GLchar msg[len];
				glGetShaderInfoLog( shader_handle, len, &len, msg );
				std::cout << "**** SHADER ERR '" << shader_type << "'" << std::endl;
				std::cout <<  msg << std::endl;
				return false;
			}

			return true;
		}

		bool create_program( GLuint vert_handle, GLuint frag_handle, GLuint & prog_handle ) const
		{
			prog_handle = glCreateProgram();
			glAttachShader( prog_handle, vert_handle );
			glAttachShader( prog_handle, frag_handle );

			GLint success;

			glLinkProgram( prog_handle );
			glGetProgramiv( prog_handle, GL_LINK_STATUS, &success );
			if ( success != GL_TRUE )
				return false;

			glValidateProgram( prog_handle );
			glGetProgramiv( prog_handle, GL_VALIDATE_STATUS, &success );

			if ( success != GL_TRUE )
			{
				GLsizei len;
				glGetProgramiv( prog_handle, GL_INFO_LOG_LENGTH, &len );
				GLchar msg[len];
				glGetProgramInfoLog( prog_handle, len, &len, msg );
				std::cout << "**** PROGRAM ERR" << std::endl;
				std::cout <<  msg << std::endl;
				return false;
			}

			return true;
		}
	};
}

#endif //A_RETRO_UI_GRAPHICS_SHADERMANAGER_OPENGL_H
