#ifndef A_RETRO_UI_UI_SYSTEMGRID_H
#define A_RETRO_UI_UI_SYSTEMGRID_H

#include <memory>
#include <cmath>
#include <iostream>
#include <vector>

#include "platform.h"
#include "Ecs_System.h"
#include "Ui_Context.h"
#include "Ui_ComponentGrid.h"
#include "Ui_ComponentTransform.h"

namespace RetroUi
{
	using std::shared_ptr;

	class SystemGrid : public RetroEcs::ISystem
	{
		shared_ptr< RetroApplication::InputContext > _input;
		shared_ptr< RetroUi::Context >               _ui;
		shared_ptr< RetroUi::ComponentGrid >         _c_grid;
		shared_ptr< RetroUi::ComponentTransform >    _c_transform;

	public:
		explicit SystemGrid(
			shared_ptr< RetroApplication::InputContext > input,
			shared_ptr< RetroUi::Context > ui,
			shared_ptr< RetroUi::ComponentGrid > c_grid,
			shared_ptr< RetroUi::ComponentTransform > c_transform )
			: _input ( input )
			, _ui( ui )
			, _c_grid( c_grid )
			, _c_transform( c_transform )
		{
		}

		virtual void tick() override
		{
			// TODO Get a message or command about grid being dirty and needs relayout?

			handle_input();
			handle_layout(); // TODO Do relayout every tick for now!
		}

	private:
		void handle_input()
		{
			RetroEcs::Entity focus = _ui->focus;

			if ( !focus.id )
				return;

			// Focusable object always has a parent, check if that parent is a grid.
			u32 i_transform = _c_transform->parent( focus );
			RetroEcs::Entity e = _c_transform->_data.entity[ i_transform ];

			if ( !_c_grid->contains(e) )
				return;

			// TODO Do we want wrapping?

			handle_input_horizontal( focus );
			handle_input_vertical( e, focus );
			_ui->focus = focus;
		}

		bool handle_input_horizontal( RetroEcs::Entity& focus )
		{
			if ( !_input->horizontal )
				return false;

			u32 i_sibling = _input->horizontal > 0.f ?
				_c_transform->next_sibling( focus ) :
				_c_transform->prev_sibling( focus );

			if ( i_sibling == _c_transform->INVALID_INDEX )
				return false;

			focus = _c_transform->_data.entity[ i_sibling ];
			return true;
		}

		bool handle_input_vertical( RetroEcs::Entity e, RetroEcs::Entity& focus )
		{
			if ( !_input->vertical )
				return false;

			vector< u32 >& siblings_candidates = _input->vertical > 0.f ?
				_c_transform->_data.prev_sibling :
				_c_transform->_data.next_sibling;

			u32 i_sibling = _c_transform->lookup( focus );
			int i = _c_grid->grid_size( e ).x;

			while ( i-- )
			{
				i_sibling = siblings_candidates[ i_sibling ];
				if ( i_sibling == _c_transform->INVALID_INDEX )
					return false;
			}

			focus = _c_transform->_data.entity[ i_sibling ];
			return true;
		}

		void handle_layout()
		{
			for ( u32 i_grid = 0, n = _c_grid->size(); i_grid < n; ++i_grid )
			{
				RetroEcs::Entity e = _c_grid->_data.entity[ i_grid ];
				glm::ivec2& grid_size = _c_grid->_data.grid_size[ i_grid ];
				glm::ivec2& cell_size = _c_grid->_data.cell_size[ i_grid ];

				float grid_x = _c_transform->x( e );
				float grid_y = _c_transform->y( e );

				u32 n_cell = 0;
				u32 i_cell = _c_transform->first_child( e );
				while ( i_cell != _c_transform->INVALID_INDEX )
				{
					_c_transform->_data.x[ i_cell ] = grid_x + cell_size.x * (n_cell % grid_size.x);
					_c_transform->_data.y[ i_cell ] = grid_y - cell_size.y * (n_cell / grid_size.x);

					i_cell = _c_transform->_data.next_sibling[ i_cell ];
					++n_cell;

					if ( n_cell > 1000 )
					{
						std::cout << "[RetroUi::SystemGrid::handle_layou] ERROR BROKEN WHILE!" << std::endl;
						return;
					}
				}
			}
		}
	};
}

#endif //A_RETRO_UI_UI_SYSTEMGRID_H
