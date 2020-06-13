#include "checkbox.hpp"
#include "window.hpp"

namespace snekUI {

	void checkbox::think( ) {

		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		/* in-case our check size is lower than needed */
		if ( check_size.h < text_size.h )
			check_size.h = text_size.h;

		/* get area */
		this->check_area = renderer::rect { parent_window.cursor_pos.x, parent_window.cursor_pos.y, check_size.w, check_size.h };
		this->area = renderer::rect { parent_window.cursor_pos.x + check_area.w + 2, parent_window.cursor_pos.y, text_size.w, text_size.h };

		/* handle checking area pressing */
		if ( helpers::clicking( renderer::rect { this->check_area.x, this->check_area.y, this->check_area.w + this->area.w, this->check_area.h } ) ) {
			this->value = !this->value;
		}

		/* set new cursor pos Y */
		parent_window.cursor_pos.y += this->check_area.h + 2;

	}

	void checkbox::draw( ) {
		think( );

		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( text , parent_window.font );

		/* draw check area */
		render.filled_rect( this->check_area.x , this->check_area.y , this->check_area.w , this->check_area.h , parent_window.theme.object_color );

		/* draw check area if value is true */
		if ( this->value ) {
			const int step = 2; /* change this if you want */
			render.filled_rect( this->check_area.x + step , this->check_area.y + step , this->check_area.w - ( step * 2 ) , this->check_area.h - ( step * 2 ) , parent_window.theme.object_select_color );
		}

		/* draw text */
		render.text( this->area.x , this->area.y + ( this->check_area.h / 2 ) - ( text_size.h / 2 ) , this->text , parent_window.font , parent_window.theme.text_color );
	}

}
