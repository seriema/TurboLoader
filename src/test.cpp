#include <iostream>

#include <msdfgen.h>
#include <msdfgen-ext.h>

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "platform.h"
#include "EnvironmentFactory.h"
#include "Graphics_Renderer.h"
#include "Graphics_Renderer_SDL_OpenGL.h"
#include "Input.h"
#include "Application_Main.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"
#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Gui_Renderer.h"

static int lua_hello_world( lua_State* L )
{
	int argc = lua_gettop( L );
	int argv = lua_tointeger( L, 1 );

	printf( "Got data from Lua :: %d args :: '%d'\n", argc, argv );

	lua_pushnumber( L, argc );
	lua_pushnumber( L, argv );
	return 2; // Number of lua_pushX calls.
}

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
	//Smoke test Lua.
	printf( "¿¿ Smoke test Lua ??\n" );
	lua_State* L = luaL_newstate();
	luaL_openlibs( L );
	lua_register( L, "lua_hello_world", lua_hello_world );
	luaL_dofile( L, "hello_world.lua" );
	lua_close( L );


	// --- STARTUP ------------------------------

	printf( "¿¿ Startup environment ??\n" );
	EnvironmentFactory_SDL_OpenGL environment_factory;
	IEnvironmentManager * environment_manager = environment_factory.create_environment_manager();
	if ( environment_manager == nullptr )
		return 1;

	printf( "¿¿ Startup renderer ??\n" );
	RetroResource::BitmapCollection bitmaps;
	RetroResource::ShaderCollection shaders;
	auto texture_manager = new RetroGraphics::TextureManager_OpenGL( bitmaps );
	auto shader_manager = new RetroGraphics::ShaderManager_OpenGL( shaders );
	RetroGraphics::IRenderer * renderer = new RetroGraphics::Renderer_SDL_OpenGL( &bitmaps, &shaders, texture_manager, shader_manager );
	RetroGui::Renderer * gui_renderer = new RetroGui::Renderer( *renderer );
	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Unable to initialize OpenGL!\n%d\n", error );
		return 2;
	}

	printf( "¿¿ Startup input ??\n" );
	Input * input = new Input();

	printf( "¿¿ Startup app ??\n" );
	IApplication * app = new Application_Main( environment_manager, renderer, gui_renderer, input, bitmaps, shaders );

	// --- TEST ---------------------------------

	//Test run msdf font.
	printf( "¿¿ Test run msdfgen ??\n" );
	msdfgen_hello_world();

	//Test run input.
	printf( "¿¿ Test run input ??\n" );
	input->poll_events();

	//Test run renderer.
	printf( "¿¿ Test run renderer ??\n" );
	renderer->render();

	printf( "¿¿ Test run app ??\n" );
	app->tick();

	// --- SHUTDOWN -----------------------------

	printf( "¿¿ Shutdown app ??\n" );
	delete app;

	//Shutdown input.
	printf( "¿¿ Shutdown input ??\n" );
	delete input;

	printf( "¿¿ Shutdown renderer ??\n" );
	delete renderer;

	//Shutdown renderer.
	printf( "¿¿ Shutdown environment ??\n" );
	environment_factory.destroy_environment_manager( environment_manager );

	// --- EXIT ---------------------------------

	//Test successful.
	printf( "¿¿ Test successful ??\n" );
	return 0;
}
