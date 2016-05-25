#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include "platform.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"
#include "Resource_BitmapLoader.h"
#include "Resource_ShaderLoader.h"
#include "EnvironmentFactory.h"
#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Graphics_Renderer_SDL_OpenGL.h"
#include "Application_Main.h"

#define A_RETRO_UI_USE_SDL 1
#define A_RETRO_UI_USE_OPENGL 1

#if defined(A_RETRO_UI_USE_SDL) && defined(A_RETRO_UI_USE_OPENGL)

static void msdfgen_hello_world()
{
	msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
	if ( ft )
	{
		msdfgen::FontHandle* font = loadFont( ft, "OpenSans.ttf" );
		if ( font )
		{
			msdfgen::Shape shape;
			if ( loadGlyph( shape, font, 'A' ))
			{
				shape.normalize();
				// max. angle
				edgeColoringSimple( shape, 3.0 );
				// image width, height
				msdfgen::Bitmap<msdfgen::FloatRGB> msdf( 32, 32 );
				// range, scale, translation
				generateMSDF( msdf, shape, 4.0, 1.0, msdfgen::Vector2( 4.0, 4.0 ));
			}
			destroyFont( font );
		}
		deinitializeFreetype( ft );
	}
}

int main( int argc, char* args[] )
{
	std::cout << "STARTING IN: " << args[ 0 ] << std::endl;
	std::cout << "Initializing..." << std::endl;

	// Just make sure it runs.
	msdfgen_hello_world();

	// Setup env.

	EnvironmentFactory_SDL_OpenGL environment_factory;
	IEnvironmentManager * environment_manager = environment_factory.create_environment_manager();

	if ( environment_manager == nullptr )
		return 1;

	// Setup data structures.

	RetroResource::HandleManager handle_manager;
	RetroResource::BitmapCollection bitmaps;
	RetroResource::ShaderCollection shaders;
	std::vector< RetroResource::Handle > bitmap_handles;
	std::vector< RetroResource::Handle > shader_handles;

	// Load base bitmap resources.
	{

		RetroResource::BitmapLoader bitmap_loader( handle_manager, bitmaps );

		std::vector< const char * > names = { "img_test_a", "img_test_b" };
		std::vector< const char * > paths = { "./res/img_test.bmp", "./res/img_test.dds" };
		u32 size = names.size();
		//bitmap_handles.reserve( size );
		bitmap_handles.resize( size );
		u32 bitmap_handles_size = bitmap_loader.load( names.data(), paths.data(), bitmap_handles.data(), size );
	}

	// Load base shader resources.
	{
		RetroResource::ShaderLoader shader_loader( handle_manager, shaders );

		std::vector< const char * > names = { "debug" };
		std::vector< const char * > paths = { "./res/debug" };
		u32 size = names.size();
		//shader_handles.reserve( size );
		shader_handles.resize( size );
		u32 shader_handles_size = shader_loader.load( names.data(), paths.data(), shader_handles.data(), size );
	}

	// Setup renderering env.

	auto texture_manager = new RetroGraphics::TextureManager_OpenGL( bitmaps );
	auto shader_manager = new RetroGraphics::ShaderManager_OpenGL( shaders );
	texture_manager->load( bitmap_handles.data(), bitmap_handles.size() );
	shader_manager->load( shader_handles.data(), shader_handles.size() );
	RetroGraphics::IRenderer * renderer = new RetroGraphics::Renderer_SDL_OpenGL( &bitmaps, &shaders, texture_manager, shader_manager );

	// Run app.

	//IInputManager* input_manager = new InputManager_SDL();
	Input * input = new Input();
	IApplication* app = new Application_Main( environment_manager, renderer, input );

	app->loop();

	texture_manager->unload( bitmap_handles.data(), bitmap_handles.size() );

	delete app;
	delete input;
	delete renderer;
	delete texture_manager;

	// Unload base resources.
	{
		RetroResource::BitmapLoader bitmap_loader( handle_manager, bitmaps );
		RetroResource::ShaderLoader shader_loader( handle_manager, shaders );
		bitmap_loader.unload( bitmap_handles.data(), bitmap_handles.size() );
		shader_loader.unload( shader_handles.data(), shader_handles.size() );
	}

	// Destroy env.

	environment_factory.destroy_environment_manager( environment_manager );

	return 0;
}

#else

int main( int argc, char* args[] )
{
	std::cout << "Wrong environment! Only SDL + OpenGL supported!" << std::endl;
	return 2;
}

#endif
