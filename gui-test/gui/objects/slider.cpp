#include <algorithm>
#include "slider.hpp"
#include "window.hpp"

namespace snekUI {
	/* TIP: This has many problems if minimum is not 0/is negative. */

	float value_per_pixel( int min , int max , int size ) {
		return float( ( max + 1 ) - min ) / size;
	}

	void slider::think( ) {
		this->type = object_slider;

		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		/* in-case our slider size is lower than needed */
		if ( this->slider_size.h < text_size.h )
			this->slider_size.h = text_size.h + 1;

		/* get text position we're going to use to draw label text! */
		this->text_pos = parent_window.cursor_pos;

		/* get slider rect which we're going to use to draw! */
		this->slider_rect = { parent_window.cursor_pos.x, parent_window.cursor_pos.y + text_size.h + 2, this->slider_size.w, this->slider_size.h };

		/* handle value */
		if ( render.mouse_in_region( this->slider_rect ) && GetAsyncKeyState( VK_LBUTTON ) ) {
			renderer::pos mouse_pos;
			render.mouse_pos( mouse_pos );

			this->value = static_cast< int >( ( ( mouse_pos.x - this->slider_rect.x ) * value_per_pixel( this->min , this->max , this->slider_size.w ) ) );
		}

		/* clamp value, to not f*ck up */
		this->value = std::clamp( this->value , this->min , this->max );

		/* set new cursor pos Y */
		parent_window.cursor_pos.y += text_size.h + this->slider_rect.h + 8;
	}

	void slider::draw( ) {
		think( );

		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		/* draw label text */
		render.text( this->text_pos , this->text , parent_window.font , parent_window.theme.text_color );

		/* draw main slider rectangle */
		render.filled_rect( this->slider_rect , parent_window.theme.object_color );
		render.outlined_rect( this->slider_rect , parent_window.theme.border_color );

		/* value width */
		int slide_value_width = static_cast< int >( ( this->value - this->min ) * float( this->slider_size.w ) / ( this->max - this->min ) );

		/* draw slider rectangle from start to value */
		render.filled_rect( renderer::rect { this->slider_rect.x + 1, this->slider_rect.y + 1, slide_value_width, this->slider_rect.h - 1 } , parent_window.theme.object_select_color );

		/* draw value text */
		std::string value_text = std::to_string( this->value );
		renderer::dim value_text_size = render.text_size( value_text , parent_window.font );
		render.text( { this->slider_rect.x + slide_value_width - ( value_text_size.w / 2 ), this->slider_rect.y + ( this->slider_rect.h / 2 ) - ( value_text_size.h / 2 ) } , value_text , parent_window.font , parent_window.theme.text_color );

	}

}
