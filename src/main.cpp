#include <memory>
#include <iostream>

#include <msdfgen.h>
#include <msdfgen-ext.h>

#include "platform.h"

#include "Environment_Factory.h"

#include "Application_Builder.h"

#include "Ecs_SystemTick.h"

#include "Ui_EntityFactory.h"
#include "Ui_SystemInput.h"
#include "Ui_SystemMain.h"
#include "Ui_SystemAnimator.h"
#include "Ui_SystemRenderer.h"

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

	RetroEnvironment::Factory environment_factory;
	std::shared_ptr< RetroEnvironment::IManager > env = environment_factory.create();

	if ( !env )
		return 1;

	std::shared_ptr< RetroApplication::Application > app;
	{
		RetroApplication::Builder builder;

		builder.env( env );

		builder.add< RetroUi::ComponentTransform >( 16 );
		builder.add< RetroUi::ComponentRender >( 16 );
		builder.add< RetroUi::EntityFactory,
				RetroResource::BitmapCollection,
				RetroResource::MeshLoader,
				RetroEcs::EntityManager,
				RetroGraphics::IShaderManager,
				RetroGraphics::ITextureManager,
				RetroGraphics::IMeshManager,
				RetroUi::ComponentTransform,
				RetroUi::ComponentRender >();

		builder.system< RetroEcs::SystemTick,
				RetroEnvironment::IManager,
				RetroEcs::TimeRaw >();

		builder.system< RetroUi::SystemInput,
				Input,
				RetroApplication::StayAlive >();

		builder.system< RetroUi::SystemMain,
				RetroResource::IPackageLoader,
				RetroResource::PackageCollection,
				RetroGraphics::IShaderManager,
				RetroGraphics::ITextureManager,
				RetroUi::EntityFactory >();

		builder.system< RetroUi::SystemAnimator,
				RetroEcs::Time,
				RetroUi::ComponentTransform >();

		builder.system< RetroUi::SystemRenderer,
				RetroEnvironment::IManager,
				RetroUi::ComponentTransform,
				RetroUi::ComponentRender >();

		app = builder.build();
	}

	app->run();
	environment_factory.destroy( env );

	return 0;
}

#else

int main( int argc, char* args[] )
{
	std::cout << "Wrong environment! Only SDL + OpenGL supported!" << std::endl;
	return 2;
}

#endif
