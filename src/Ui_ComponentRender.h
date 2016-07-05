#ifndef A_RETRO_UI_UI_COMPONENT_RENDER_H
#define A_RETRO_UI_UI_COMPONENT_RENDER_H

#include <vector>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "platform.h"
#include "Ecs_Component.h"
#include "Graphics_RenderKey.h"
#include "Graphics_Renderer.h"

namespace RetroUi
{
	using std::vector;
	using RetroEcs::Entity;
	using RetroGraphics::RenderKey;
	using RetroGraphics::RenderCommand;


	struct RenderData_Draw
	{
		RetroGraphics::RenderFunc func;
		u32 vbo; // vbo contains attributes such as vert,norm,color, these must then also exist in shader.
		u32 shader;
		u32 bitmap;
		glm::vec2 size;
		glm::mat4 mvp;
	};


	class ComponentRender : public RetroEcs::Component< ComponentRender >
	{
	public:
		struct InstanceData
		{
			vector< Entity >        entity;
			vector< RenderKey >     key;
			vector< RenderCommand > data;
		} _data;

		ComponentRender( u32 capacity ) : Component( capacity )
		{
			_data.entity.resize( capacity );
			_data.key.resize( capacity );
			_data.data.resize( capacity );
		}

		void _copy( unsigned i, unsigned i_dst, ComponentRender::InstanceData& data_dst )
		{
			data_dst.entity[ i_dst ] = _data.entity.at( i );
			data_dst.key[ i_dst ] = _data.key.at( i );
			data_dst.data[ i_dst ] = _data.data.at( i );
		}

		inline const RenderKey key( Entity e ) { return get( _data.key, e ); }
		inline void set_key( Entity e, RenderKey key ) { set( _data.key, e, key ); }

		inline const RenderCommand& data( Entity e ) { return get( _data.data, e ); }
		inline void set_data( Entity e, RenderCommand& data ) { set( _data.data, e, data ); }
	};
}

#endif //A_RETRO_UI_UI_COMPONENT_RENDER_H
