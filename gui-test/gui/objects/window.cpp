#include <algorithm>
#include "window.hpp"
#include "checkbox.hpp"
#include "combobox.hpp"

namespace snekUI {

	void window::think( ) {
		this->type = object_window;

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
				if ( render.mouse_in_region( this->pos.x , this->pos.y , this->dim.w , this->theme.drag_title ? this->theme.titlebar_size : this->dim.h ) ) {
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
				}

				m_pressed = m_held;
			}
		}

		/* set actual cursor position */
		this->cursor_pos = this->pos;
	}

	void window::draw( ) {

		/* think before drawing */
		think( );

		/* check if we have the menu opened */
		if ( !this->opened ) return;

		/* draw menu background */
		render.filled_rect( this->pos , this->dim , this->theme.background_color );

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

		/* adjust cursor X pos */
		this->cursor_pos.x += 4;

		/* draw objects */
		std::for_each(
			objects.begin( ) ,
			objects.end( ) ,
			[ ] ( std::shared_ptr< object >& child ) {
				child->draw( );
			}
		);
	}

	void* window::find_obj( std::string obj_name , object_type obj_type ) {
		for ( auto& obj : this->objects ) {
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
				}
			}
		}

		return nullptr;
	}
}
