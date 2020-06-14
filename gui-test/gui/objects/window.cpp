#include <algorithm>
#include "window.hpp"
#include "checkbox.hpp"
#include "combobox.hpp"
#include "tab.hpp"
#include "groupbox.hpp"
#include "slider.hpp"
#include "button.hpp"
#include "label.hpp"

namespace snekUI {

	void window::think( ) {

		/* get screen size */
		renderer::dim screen_size;
		render.screen_size( screen_size );

		/* get mouse pos */
		renderer::pos mouse_pos;
		render.mouse_pos( mouse_pos );

		/* in-case title-bar size is not enough, clamp it */
		renderer::dim title_text_size = render.text_size( this->title , this->title_font );
		if ( this->theme.titlebar_size < title_text_size.h )
			this->theme.titlebar_size = title_text_size.h + 1;

		/* dragging system */
		if ( this->theme.draggable ) {
			static bool m_dragging = false;
			bool m_click = false;

			static decltype( this->pos ) dragging_pos { 0, 0 };

			if ( this->opened ) {
				/* drag menu etc. */
				if ( GetAsyncKeyState( VK_LBUTTON ) ) {
					m_click = true;
				}

				if ( m_dragging && !m_click )
				{
					m_dragging = false;
				}

				if ( m_dragging && m_click )
				{
					this->pos.x = mouse_pos.x - dragging_pos.x;
					this->pos.y = mouse_pos.y - dragging_pos.y;
				}

				/* if we're 'dragging' */
				if ( helpers::hovering( this->pos.x , this->pos.y , this->dim.w , this->theme.drag_title ? this->theme.titlebar_size : this->dim.h ) ) {
					m_dragging = true;
					dragging_pos.x = mouse_pos.x - this->pos.x;
					dragging_pos.y = mouse_pos.y - this->pos.y;
				}
			}
		}

		/* clamp window pos on screen */ {
			if ( this->pos.x < 0 )
				this->pos.x = 0;

			if ( this->pos.y < 0 )
				this->pos.y = 0;

			if ( ( this->pos.x + this->dim.w ) > screen_size.w )
				this->pos.x = screen_size.w - this->dim.w;

			if ( ( this->pos.y + this->dim.h ) > screen_size.h )
				this->pos.y = screen_size.h - this->dim.h;
		}

		/* toggle open */
		if ( this->key != -1 ) {
			static bool m_pressed = false;
			const bool m_held = GetAsyncKeyState( this->key );
			if ( m_pressed != m_held ) {
				if ( m_held ) {
					this->opened = !this->opened;

					/* re-activate input if needed */
					if ( this->opened )
						helpers::g_input = true;
				}

				m_pressed = m_held;
			}
		}

		/* set actual cursor position */
		this->cursor_pos = this->pos;

		/* set area ( and actually here is the first time we set it ) */
		this->area = { this->pos.x, this->pos.y, this->dim.w, this->dim.h };
	}

	void window::draw( ) {

		/* think before drawing */
		think( );

		/* in case we have multiple windows */
		helpers::finished_input_frame = false;
		this->render_overlay = false;
		this->handle_keyboard = false;

		/* check if we have the menu opened */
		if ( !this->opened ) {
			this->render_overlay = false;
			this->handle_keyboard = false;
			helpers::g_input = false;
			return;
		}

		/* draw menu background */
		render.filled_rect( this->area , this->theme.background_color );

		/* title-bar */ {
			/*
			   TIP: we draw this ON the menu, not outside of it, which can mean that the alpha will be bad
			   We can first draw title-bar and then set 'this->pos' Y + titlebar_size and dimension height - titlebar_size
			*/

			render.filled_rect( this->pos , { this->dim.w , this->theme.titlebar_size } , this->theme.titlebar_color );

			/* centered title text */
			renderer::dim title_text_size = render.text_size( this->title , this->title_font );
			render.text( this->pos.x + ( this->dim.w / 2 ) - ( title_text_size.w / 2 ) , this->pos.y + ( this->theme.titlebar_size / 2 ) - ( title_text_size.h / 2 ) , this->title , this->title_font , this->theme.titletext_color );

			/* set next cursor pos */
			this->cursor_pos = this->pos; /* start */
			this->cursor_pos.y += this->theme.titlebar_size * 1.25; /* add title bar size and a quarter to Y cursor pos */
		}

		/* move objects inside */
		this->cursor_pos.x += this->theme.column_spacing;

		/* get tabs */
		std::vector< std::shared_ptr< tab > > tabs;
		std::for_each( objects.begin( ) , objects.end( ) , [ & ] ( std::shared_ptr< object >& obj ) {
			if ( obj->type == object_tab ) {
				tabs.push_back( std::static_pointer_cast< tab >( obj ) );
			}
			} );

		/* handle tabs */
		int tab_separate_x = 2;
		int tab_render_pos_x = this->cursor_pos.x;
		int add_cursor_pos_y = 0;
		std::for_each( tabs.begin( ) , tabs.end( ) , [ & ] ( std::shared_ptr< tab >& o_tab ) {
			int tab_render_width = ( ( this->dim.w - ( this->theme.column_spacing * 2 ) ) / int( tabs.size( ) ) ) - tab_separate_x;
			int tab_render_height = this->theme.titlebar_size;
			renderer::rect tab_rect = renderer::rect { tab_render_pos_x, this->cursor_pos.y, tab_render_width, tab_render_height };

			/* handle selecting tab */
			if ( helpers::clicking( tab_rect ) ) {

				/* deselect all */
				std::for_each( tabs.begin( ) , tabs.end( ) , [ & ] ( std::shared_ptr< tab > tab ) {
					tab->selected = false;
					} );

				/* select actual tab */
				o_tab->selected = true;
			}

			/* draw tab button background */
			render.filled_rect( tab_rect , this->theme.titlebar_color );

			/* draw tab button title */
			renderer::dim tab_title_text_size = render.text_size( o_tab->title , this->title_font );
			render.text( renderer::pos { tab_render_pos_x + ( tab_render_width / 2 ) - ( tab_title_text_size.w / 2 ), this->cursor_pos.y + ( tab_render_height / 2 ) - ( tab_title_text_size.h / 2 ) } , o_tab->title , this->title_font , o_tab->selected ? this->theme.object_select_color : this->theme.titletext_color );

			/* set next tab render pos X */
			tab_render_pos_x += tab_render_width + ( tab_separate_x * 2 );

			/* set add_cursor_pos_y because we will want to move new objects down */
			add_cursor_pos_y = tab_render_height;

			} );

		/* move new objects down */
		this->cursor_pos.y += add_cursor_pos_y + this->theme.object_spacing;

		/* draw objects inside window */
		render.clip( this->area , [ = ] ( ) {
			std::for_each(
				this->objects.begin( ) ,
				this->objects.end( ) ,
				[ ] ( std::shared_ptr< object >& obj ) {
					obj->draw( );
				}
			);
			} );

		/* draw overlays */
		if ( this->render_overlay && this->overlay_func )
			this->overlay_func( );

		/* handle click_switch because of some bugs */
		helpers::click_switch = false;
		if ( !helpers::click_switch && GetAsyncKeyState( VK_LBUTTON ) )
			helpers::click_switch = true;
	}

	void* window::find_obj( const std::string& tab_name , const std::string& group_name , const std::string& obj_name , object_type obj_type ) {
		for ( auto& obj_window : this->objects ) {
			if ( obj_window->type == object_tab ) {
				auto obj_tab = std::static_pointer_cast< tab >( obj_window );
				if ( obj_tab->title == tab_name ) {
					for ( auto& tab_obj : obj_tab->objects ) {
						if ( tab_obj->type == object_group ) {
							auto obj_group = std::static_pointer_cast< group >( tab_obj );
							if ( obj_group->title == group_name ) {
								for ( auto& obj : obj_group->objects ) {
									if ( obj->type == obj_type ) {
										switch ( obj->type ) {
										case object_checkbox: {

											auto as_checkbox = std::static_pointer_cast< checkbox >( obj );
											if ( as_checkbox->text == obj_name )
												return &as_checkbox->value;

										} break;

										case object_combobox: {

											auto as_checkbox = std::static_pointer_cast< combobox >( obj );
											if ( as_checkbox->text == obj_name )
												return &as_checkbox->value;

										} break;

										case object_slider: {

											auto as_slider = std::static_pointer_cast< slider >( obj );
											if ( as_slider->text == obj_name )
												return &as_slider->value;

										} break;

										case object_button: {

											auto as_button = std::static_pointer_cast< button >( obj );
											if ( as_button->text == obj_name )
												return &as_button->button_func;

										} break;

										case object_label: {

											auto as_label = std::static_pointer_cast< label >( obj );
											if ( as_label->text == obj_name )
												return &as_label->text;

										} break;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return nullptr;
	}
}
