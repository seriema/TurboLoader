#ifndef A_RETRO_UI_GRAPHICS_SHADERMANAGER_OPENGL_H
#define A_RETRO_UI_GRAPHICS_SHADERMANAGER_OPENGL_H

#include <algorithm>
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
			for ( u32 j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				u32 i = _shaders.handle_index.at( handle.id );
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

				link_vertex_attributes( prog_handle );

				glDeleteShader( vert_handle );
				glDeleteShader( frag_handle );

				_handles.insert( { handle.id, prog_handle } );

				std::cout << "[shader manager opengl] loaded: (" << handle.id << ") '" << _shaders.name[ i ] << "' " << "gl handle '" << prog_handle << "'" << std::endl;
			}
		}

		virtual void unload( RetroResource::Handle * handles, const u32 size = 1 ) override
		{
			for ( u32 j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				glDeleteProgram( _handles.at( handle.id ) );
				_handles.erase( handle.id );
			}
		}

		void bind( const RetroResource::Handle handle ) const
		{
			u32 prog_handle = _handles.at( handle.id );
			glUseProgram( prog_handle );
		}

		inline u32 lookup( const std::string& name ) const
		{
			u32 index = _shaders.name_index.at( name );
			RetroResource::Handle handle = _shaders.handle[ index ];
			return _handles.at( handle.id );
		}

		inline u32 lookup( const RetroResource::Handle handle ) const
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
				GLchar msg[4096];
				len = std::min(len, 4096);
				glGetShaderInfoLog( shader_handle, len, &len, msg );
				std::cout << "[RetroGraphics::ShaderManager_OpenGL::create_shader] shader error '" << shader_type << "':" << std::endl;
				std::cout << "    " << msg << std::endl;
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
			{
				GLsizei len;
				glGetProgramiv( prog_handle, GL_INFO_LOG_LENGTH, &len );
				GLchar msg[4096];
				len = std::min(len, 4096);
				glGetProgramInfoLog( prog_handle, len, &len, msg );
				std::cout << "[RetroGraphics::ShaderManager_OpenGL::create_program] program link error:" << std::endl;
				std::cout << "    " << msg << std::endl;
				return false;
			}

			glValidateProgram( prog_handle );
			glGetProgramiv( prog_handle, GL_VALIDATE_STATUS, &success );

			if ( success != GL_TRUE )
			{
				GLsizei len;
				glGetProgramiv( prog_handle, GL_INFO_LOG_LENGTH, &len );
				GLchar msg[4096];
				len = std::min(len, 4096);
				glGetProgramInfoLog( prog_handle, len, &len, msg );
				std::cout << "[RetroGraphics::ShaderManager_OpenGL::create_program] program validation error:" << std::endl;
				std::cout << "    " << msg << std::endl;
				return false;
			}

			return true;
		}

		void link_vertex_attributes( GLuint prog_handle )
		{
			GLint n = GL_ZERO;
			GLsizei len = GL_ZERO;
			GLint size = GL_ZERO;
			GLenum type = GL_ZERO;
			GLchar name[ 128 ];
			GLsizei max_len = sizeof( name ) - 1;

			glGetProgramiv( prog_handle, GL_ACTIVE_ATTRIBUTES, &n );

			for( GLint i = 0; i < n; ++i )
			{
				glGetActiveAttrib( prog_handle, GLuint(i), max_len, &len, &size, &type, name );
				name[ len ] = GL_ZERO;
				glEnableVertexAttribArray( glGetAttribLocation( prog_handle, name ) );
			}
		}
	};
}

#endif //A_RETRO_UI_GRAPHICS_SHADERMANAGER_OPENGL_H
