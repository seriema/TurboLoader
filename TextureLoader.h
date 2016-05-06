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

		FIBITMAP* bgra = load_FreeImage( filepath.c_str(), w, h, imgdata );
		GLubyte* rgba = convert_bgra_to_rgba( w, h, bgra );
		GLuint texture_handle = load_OpenGL( w, h, rgba );
		unload_FreeImage( bgra );
		delete [] rgba;

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

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType( filepath, 0 );
		FIBITMAP* img = FreeImage_Load( format, filepath );

		{
			FIBITMAP* temp = img;
			img = FreeImage_ConvertTo32Bits( img );
			FreeImage_Unload( temp );
		}

		w = FreeImage_GetWidth( img );
		h = FreeImage_GetHeight( img );

		printf( "Texture file: %s\n", filepath );
		printf( "Texture size: %d x %d\n", w, h );

		//imgdata = FreeImage_GetBits( img ); // TODO if img size isn't power of 2 then use FreeImage_ConvertToRawBits!
		return img;
	}

	bool unload_FreeImage( FIBITMAP* img )
	{
		FreeImage_Unload( img );
		return true; // TODO return false if img doesn't exist.
	}

	GLubyte* convert_bgra_to_rgba( int w, int h, FIBITMAP* imgdata )
	{
		// https://www.opengl.org/discussion_boards/showthread.php/163929-image-loading?p=1158293#post1158293
		BYTE* fib = FreeImage_GetBits( imgdata );
		GLubyte* texture = new GLubyte[4*w*h];
		auto square = w * h;

		for(int j = 0; j < square; j++){
			texture[j*4+0]= fib[j*4+2];
			texture[j*4+1]= fib[j*4+1];
			texture[j*4+2]= fib[j*4+0];
			texture[j*4+3]= fib[j*4+3];
		}

		return texture;
	}

	GLuint load_OpenGL( int w, int h, GLubyte *imgdata )
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
