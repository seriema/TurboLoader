#ifndef A_RETRO_UI_UI_SYSTEM_MAIN_H
#define A_RETRO_UI_UI_SYSTEM_MAIN_H

#include <cmath>

#include "platform.h"
#include "Ecs_EntityManager.h"
#include "Ecs_System.h"
#include "Ui_Component_Bitmap.h"
#include "Ui_Component_Transform.h"
#include "Resource_BitmapCollection.h"
#include "Resource_ShaderCollection.h"

namespace RetroUi
{
	using RetroEcs::EntityManager;
	using RetroEcs::Entity;

	class SystemMain : public RetroEcs::System
	{
		EntityManager&      _em;
		ComponentBitmap&    _bitmap;
		ComponentTransform& _transform;
		RetroResource::BitmapCollection & _bitmaps;
		RetroResource::ShaderCollection & _shaders;

		std::vector< Entity > _entities;

		void create( std::string bitmap, std::string shader, float x, float y )
		{
			RetroResource::Handle bitmap_handle = _bitmaps.handle[ _bitmaps.name_index.at(bitmap) ];
			RetroResource::Handle shader_handle = _shaders.handle[ _shaders.name_index.at(shader) ];

			Entity e = _em.create();

			_bitmap.create( e );
			_bitmap.set_bitmap( e, bitmap_handle );
			_bitmap.set_shader( e, shader_handle );

			_transform.create( e );
			_transform.set_x( e, x );
			_transform.set_y( e, y );
			_transform.set_z( e, 0.0f );

			_entities.push_back( e );
		}
		
	public:
		explicit SystemMain(
				EntityManager& em,
				ComponentBitmap & bitmap,
				ComponentTransform & transform,
				RetroResource::BitmapCollection & bitmaps,
				RetroResource::ShaderCollection & shaders )
			: _em( em )
			, _bitmap( bitmap )
			, _transform( transform )
			, _bitmaps( bitmaps )
			, _shaders( shaders )
		{
			create( "jp", "debug", 0.3f, 0.3f );
			create( "jp", "debug", -0.4f, -0.4f );
			create( "jp", "debug", 0.8f, -0.4f );

			create( "jb", "debug", -0.9f, -0.1f );
			create( "jb", "debug", 0.1f, -0.8f );
		}

		virtual ~SystemMain() override
		{
			for ( auto e : _entities )
			{
				_bitmap.destroy( e );
				_transform.destroy( e );
			}
		}

		virtual void tick( u32 dt ) override
		{
			// TODO Poll gui interaction events ???

			for ( u32 i = 0, n = _transform.size(); i < n; ++i )
			{
				float new_x = _transform._data.x[ i ] + 0.0005f * dt;
				_transform._data.x[ i ] = std::fmod( 1.1f + new_x, 2.2f ) - 1.1f;
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEM_MAIN_H
