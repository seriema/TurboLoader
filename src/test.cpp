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
#include "Environment_Factory.h"
#include "Graphics_Renderer.h"
#include "Application_Builder.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"
#include "Graphics_TextureManager_OpenGL.h"
#include "Graphics_ShaderManager_OpenGL.h"
#include "Resource_HandleManager.h"
#include "Resource_BitmapLoader.h"
#include "Resource_ShaderLoader.h"
#include "Ui_SystemInput.h"

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

class SystemTest : public RetroEcs::ISystem
{
	shared_ptr< RetroApplication::InputContext > _input;
public:
	SystemTest( shared_ptr<RetroApplication::InputContext> input )
		: _input( input )
	{}

	void tick()
	{
		_input->quit = true;
	}
};

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
	RetroEnvironment::Factory env_factory;
	shared_ptr< RetroEnvironment::IManager > env = env_factory.create();
	if ( !env )
		return 1;

	printf( "¿¿ Load resources ??\n" );
	RetroResource::HandleManager handle_manager;
	RetroResource::BitmapCollection bitmaps;
	RetroResource::ShaderCollection shaders;
	std::vector< RetroResource::Handle > bitmap_handles;
	std::vector< RetroResource::Handle > shader_handles;
	printf( "¿¿¿ Load resources: bitmaps ???\n" );
	{
		RetroResource::BitmapLoader bitmap_loader( handle_manager, bitmaps );

		std::vector< std::string > names = { "img_test_a", "img_test_b", "jp", "jb" };
		std::vector< std::string > paths = { "./res/bitmap/img_test.bmp", "./res/bitmap/img_test.dds", "./res/bitmap/jp.png", "./res/bitmap/jb.png" };
		u32 size = names.size();
		bitmap_handles.resize( size );
		u32 bitmap_handles_size = bitmap_loader.load( names.data(), paths.data(), bitmap_handles.data(), size );
	}
	printf( "¿¿¿ Load resources: shaders ???\n" );
	{
		RetroResource::ShaderLoader shader_loader( handle_manager, shaders );

		std::vector< std::string > names = { "debug" };
		std::vector< std::string > paths = { "./res/shader/debug" };
		u32 size = names.size();
		//shader_handles.reserve( size );
		shader_handles.resize( size );
		u32 shader_handles_size = shader_loader.load( names.data(), paths.data(), shader_handles.data(), size );
	}
	printf( "¿¿¿ Load resources: managers ???\n" );
	auto texture_manager = new RetroGraphics::TextureManager_OpenGL( bitmaps );
	auto shader_manager = new RetroGraphics::ShaderManager_OpenGL( shaders );
	texture_manager->load( bitmap_handles.data(), bitmap_handles.size() );
	//shader_manager->load( shader_handles.data(), shader_handles.size() );

	GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Unable to initialize OpenGL!\n%d\n", error );
		return 2;
	}

	printf( "¿¿ Build app ??\n" );
	std::shared_ptr< RetroApplication::Application > app;
	{
		RetroApplication::Builder builder;
		builder.env( env );
		builder.system< RetroUi::SystemInput, RetroApplication::InputContext >();
		builder.system< SystemTest, RetroApplication::InputContext >();
		app = builder.build();
	}

	// --- TEST ---------------------------------

	//Test run msdf font.
	printf( "¿¿ Test run msdfgen ??\n" );
	msdfgen_hello_world();

	//Test run renderer.
	printf( "¿¿ Test run renderer ??\n" );

	printf( "¿¿ Test run app ??\n" );
	app->run();

	// --- SHUTDOWN -----------------------------

	printf( "¿¿ Shutdown app ??\n" );
	app.reset();

	printf( "¿¿ Unload resources ??\n" );
	texture_manager->unload( bitmap_handles.data(), bitmap_handles.size() );
	//shader_manager->unload( shader_handles.data(), shader_handles.size() );
	delete texture_manager;
	delete shader_manager;
	// Unload base resources.
	{
		RetroResource::BitmapLoader bitmap_loader( handle_manager, bitmaps );
		RetroResource::ShaderLoader shader_loader( handle_manager, shaders );
		bitmap_loader.unload( bitmap_handles.data(), bitmap_handles.size() );
		shader_loader.unload( shader_handles.data(), shader_handles.size() );
	}

	printf( "¿¿ Shutdown environment ??\n" );
	env_factory.destroy( env );

	// --- EXIT ---------------------------------

	//Test successful.
	printf( "¿¿ Test successful ??\n" );
	return 0;
}
