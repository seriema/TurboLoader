#ifndef A_RETRO_UI_GRAPHICS_TEXTUREMANAGER_OPENGL_H
#define A_RETRO_UI_GRAPHICS_TEXTUREMANAGER_OPENGL_H

#include <unordered_map>
#include <string>

// TODO Remove free image dependency from this file!
#include <FreeImage.h>
#include <glm/vec2.hpp>

#include "platform.h"
#include "Graphics_TextureManager.h"
#include "Resource_BitmapCollection.h"

namespace RetroGraphics
{
	// 32 slots for loaded textures in a data structure.
	// Every time a texture is used a handle is taken from the front.
	// If that handle is already in use then unload the current texture.
	// Add the handle to end of list.
	// This accumulates the most often used textures at the end of the list.

	const u8 MAX_NUM_TEXTURES = 32;


	class TextureManager_OpenGL : public ITextureManager
	{
		RetroResource::BitmapCollection & _bitmaps;
		std::unordered_map< u32, u32 >    _handles;

	public:
		TextureManager_OpenGL( RetroResource::BitmapCollection & bitmaps )
			: _bitmaps( bitmaps )
		{
		}

		virtual ~TextureManager_OpenGL() override
		{
			// TODO make sure all textures are unloaded.
		}

		virtual void load( RetroResource::Handle * handles, const u32 size = 1 ) override
		{
			for ( u32 j = 0; j < size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				u32 i = _bitmaps.handle_index.at( handle.id );
				glm::vec2 size = _bitmaps.size[ i ];
				void * bitmap = _bitmaps.resource[ i ];

				u32 texture_handle;
				glGenTextures( 1, &texture_handle );
				glBindTexture( GL_TEXTURE_2D, texture_handle );

				//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );// GL_CLAMP );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );// GL_CLAMP );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

				// http://stackoverflow.com/questions/11042027/glpixelstoreigl-unpack-alignment-1-disadvantages
				// glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

				// http://docs.gl/es2/glTexImage2D
				void * bits = get_bits((u32)size.x, (u32)size.y, (FIBITMAP*)bitmap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
				//glGenerateMipmap( GL_TEXTURE_2D );

				// TODO make sure handle isnt already in _handles.
				_handles.insert( { handle.id, texture_handle } );

				GLenum err = glGetError();
				if( err != GL_NO_ERROR )
					std::cout << "[texture manager opengl] gl error loading '" << _bitmaps.name[ i ] << "': " << err << std::endl;
				else
					std::cout << "[texture manager opengl] loaded: (" << handle.id << ") '" << _bitmaps.name[ i ] << "' :: gl handle '" << texture_handle << "'" << std::endl;
			}
		}

		virtual void unload( RetroResource::Handle * handles, const u32 size = 1 ) override
		{
			for ( u32 j = 0; j <  size; ++j )
			{
				RetroResource::Handle handle = handles[ j ];
				glDeleteTextures( 1, &_handles.at( handle.id ) );
			}
		}

		void bind( const RetroResource::Handle handle ) const
		{
			u32 texture_handle = _handles.at( handle.id );
			glBindTexture( GL_TEXTURE_2D, texture_handle );
		}

		inline u32 lookup( const std::string& name ) const
		{
			u32 index = _bitmaps.name_index.at( name );
			RetroResource::Handle handle = _bitmaps.handle[ index ];
			return _handles.at( handle.id );
		}

		inline u32 lookup( const RetroResource::Handle handle ) const
		{
			return _handles.at( handle.id );
		}

	private:
		void * get_bits( u32 w, u32 h, FIBITMAP * bitmap )
		{
			// https://www.opengl.org/discussion_boards/showthread.php/163929-image-loading?p=1158293#post1158293
			BYTE * bgra = FreeImage_GetBits( bitmap );
			GLubyte * rgba = new GLubyte[ 4 * w * h ];
			for( u32 j = 0, jSize = w * h; j < jSize; ++j )
			{
				rgba[ j * 4 + 0 ] = bgra[ j * 4 + 2 ];
				rgba[ j * 4 + 1 ] = bgra[ j * 4 + 1 ];
				rgba[ j * 4 + 2 ] = bgra[ j * 4 + 0 ];
				rgba[ j * 4 + 3 ] = bgra[ j * 4 + 3 ];
			}
			return rgba;
		}
	};
}

#endif //A_RETRO_UI_GRAPHICS_TEXTUREMANAGER_OPENGL_H
