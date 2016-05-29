#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include <msdfgen.h>
#include <msdfgen-ext.h>

#include "platform.h"
#include "Resource_FontCollection.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"
#include "Resource_PackageCollection.h"
#include "Resource_PackageLoader.h"
#include "Resource_PackageLoader_Lua.h"
#include "EnvironmentFactory.h"
#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Graphics_Renderer_SDL_OpenGL.h"
#include "Gui_Renderer.h"
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
	RetroResource::FontCollection fonts;
	RetroResource::BitmapCollection bitmaps;
	RetroResource::ShaderCollection shaders;
	RetroResource::PackageCollection packages;
	RetroResource::Handle base_package_handle;

	// Load base resources.
	{
		RetroResource::PackageLoader_Lua package_loader( handle_manager, packages, fonts, bitmaps, shaders );//, material_loader );
		package_loader.load( "./src/hello_world", base_package_handle );
	}

	// Setup renderering env.
	auto texture_manager = new RetroGraphics::TextureManager_OpenGL( bitmaps );
	auto shader_manager = new RetroGraphics::ShaderManager_OpenGL( shaders );
	{
		auto & package = packages.handle_lookup.at( base_package_handle.id );

		// TODO load font textures via handles package.fonts.data() ...

		texture_manager->load( package.fonts.data(), package.fonts.size() );
		texture_manager->load( package.bitmaps.data(), package.bitmaps.size() );
		shader_manager->load( package.shaders.data(), package.shaders.size() );
	}

	RetroGraphics::IRenderer * renderer = new RetroGraphics::Renderer_SDL_OpenGL( &bitmaps, &shaders, texture_manager, shader_manager );
	RetroGui::Renderer * gui_renderer = new RetroGui::Renderer( *renderer );

	// Run app.

	//IInputManager* input_manager = new InputManager_SDL();
	Input * input = new Input();
	IApplication* app = new Application_Main( environment_manager, renderer, gui_renderer, input, bitmaps, shaders );

	app->loop();

	{
		auto & package = packages.handle_lookup.at( base_package_handle.id );
		texture_manager->unload( package.bitmaps.data(), package.bitmaps.size() );
		shader_manager->unload( package.shaders.data(), package.shaders.size() );
	}

	delete app;
	delete input;
	delete gui_renderer;
	delete renderer;
	delete texture_manager;
	delete shader_manager;

	// Unload base resources.
	{
		RetroResource::PackageLoader_Lua package_loader( handle_manager, packages, fonts, bitmaps, shaders );//, material_loader );
		package_loader.unload( &base_package_handle );
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
