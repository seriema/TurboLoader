#ifndef A_RETRO_UI_TEXTURELOADER_H
#define A_RETRO_UI_TEXTURELOADER_H

#include <iostream>

#include <FreeImage.h>

#include "platform.h"


class TextureLoader_FreeImage_OpenGL
{
public:
	TextureLoader_FreeImage_OpenGL()
	{
		int maxTextureSize;
		glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
		printf( "Max texture size: %d\n", maxTextureSize );
	}

	GLuint load( std::string filepath )
	{
		int w, h;
		GLubyte* imgdata;

		FIBITMAP* img = load_FreeImage( filepath.c_str(), w, h, imgdata );
//		GLuint texture_handle = load_OpenGL( imgdata, w, h );
		GLuint texture_handle = load_OpenGL( FreeImage_GetBits( img ), w, h );
		unload_FreeImage( img );




		return texture_handle;
	}

	bool unload( GLuint handle )
	{
		//TODO glDeleteTextures( ... );
		return true; // return false if not exists.
	}

private:
	FIBITMAP* load_FreeImage( const char* filepath, int& w, int& h, GLubyte* imgdata )
	{
		// BEWARE! FreeImage loads in BGR format!

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType( filepath, 0 );
		FIBITMAP* img = FreeImage_Load( formato, filepath );

		{
			FIBITMAP* temp = img;
			img = FreeImage_ConvertTo32Bits( img );
			FreeImage_Unload( temp );
		}

		w = FreeImage_GetWidth( img );
		h = FreeImage_GetHeight( img );

		printf( "Texture file: %s\n", filepath );
		printf( "Texture size: %d x %d\n", w, h );

		//imgdata = FreeImage_GetBits( img ); // TODO if img size isnt power of 2 then use FreeImage_ConvertToRawBits!
		return img;
	}

	bool unload_FreeImage( FIBITMAP* img )
	{
		FreeImage_Unload( img );
		return true; // TODO return false if img doesn't exist.
	}

	GLuint load_OpenGL( GLubyte* imgdata, int w, int h )
	{
		GLuint texture_handle;

		glGenTextures( 1, &texture_handle );
		glBindTexture( GL_TEXTURE_2D, texture_handle );

		// http://stackoverflow.com/questions/11042027/glpixelstoreigl-unpack-alignment-1-disadvantages
		// glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		// http://docs.gl/es2/glTexImage2D
		glTexImage2D(
			GL_TEXTURE_2D, 0 ,GL_RGBA,
			w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata );

		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );// GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );// GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		GLenum huboError = glGetError();
		if( huboError != GL_NO_ERROR )
		{
			printf( "Load texture error: %d\n", huboError );
		}
		else
		{
			printf( "Load texture success!\n" );
		}

		return texture_handle;
	}
};


#endif //A_RETRO_UI_TEXTURELOADER_H
