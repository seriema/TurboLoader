#ifndef A_RETRO_UI_APPLICATION_MAIN_H
#define A_RETRO_UI_APPLICATION_MAIN_H

#include <iostream>
#include <string>
#include <vector>

#include "Application.h"
#include "EnvironmentManager.h"
#include "Graphics_Renderer.h"
#include "Gui_Renderer.h"
#include "Input.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"

#include "Ecs_EntityManager.h"
#include "Ui_Component_Bitmap.h"
#include "Ui_Component_Transform.h"
#include "Ui_System_Renderer.h"
#include "Ui_System_Main.h"

class Application_Main : public IApplication
{
	IEnvironmentManager*              _environment_manager;
	RetroGraphics::IRenderer*         _renderer;
	RetroGui::Renderer*               _gui_renderer;
	Input*                            _input;
	RetroResource::BitmapCollection&  _bitmaps;
	RetroResource::ShaderCollection&  _shaders;

	RetroEcs::EntityManager           _em;
	std::vector< RetroEcs::System * > _systems;
	RetroUi::ComponentBitmap          _comp_bitmap;
	RetroUi::ComponentTransform       _comp_transform;

public:
	Application_Main(
			IEnvironmentManager * environment_manager,
			RetroGraphics::IRenderer * renderer,
			RetroGui::Renderer * gui_renderer,
			Input * input,
			RetroResource::BitmapCollection & bitmaps,
			RetroResource::ShaderCollection & shaders )
		: _environment_manager( environment_manager )
		, _renderer( renderer )
		, _gui_renderer( gui_renderer )
		, _input( input )
		, _bitmaps( bitmaps )
		, _shaders( shaders )
		, _comp_bitmap( RetroUi::ComponentBitmap( 16 ) )
		, _comp_transform( RetroUi::ComponentTransform( 16 ) )
	{
		_systems.push_back( new RetroUi::SystemMain( _em, _comp_bitmap, _comp_transform, _bitmaps, _shaders ) );
		_systems.push_back( new RetroUi::SystemRenderer( _comp_bitmap, _comp_transform, *_gui_renderer ) );
	}

	virtual ~Application_Main() override
	{
	}

	virtual void loop() override
	{
		printf("Entering main loop\n");

		uint32_t t0 = _environment_manager->get_ticks() - 17;

		while( !_input->quit_requested() )
		{
			u32 t1 = _environment_manager->get_ticks();
			u32 dt = t1 - t0;
			t0 = t1;
			tick( dt );
		}

		for ( auto system : _systems )
		{
			delete system;
		}

		_systems.clear();
	}

	virtual void tick( u32 dt ) override
	{
		_input->poll_events();

		for ( auto system : _systems )
		{
			system->tick( dt );
		}

		_renderer->render();
		_environment_manager->swap();
	}

	virtual bool is_alive() override
	{
		return true;
	}
};

#endif //A_RETRO_UI_APPLICATION_MAIN_H
