#include "button.hpp"
#include "window.hpp"

namespace snekUI {
	void button::think( )
	{
		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		/* get button size width */
#ifdef BUTTON_WIDTH_AUTO
		this->button_size.w = text_size.w + 1;
#else
		this->button_size.w = this->area.w;
#endif

		/* maximize button height if needed */
		if ( this->button_size.h < text_size.h )
			this->button_size.h = text_size.h + 1;

		/* get button area */
		this->button_area = renderer::rect { parent_window.cursor_pos.x, parent_window.cursor_pos.y, this->button_size.w, this->button_size.h };

		/* handle pressing and run function */
		if ( helpers::clicking( this->button_area ) ) {
			this->button_func( );
		}

	}

	void button::draw( )
	{
		think( );

		/* self-explenatory... */
		/* ------------------- */

		auto& parent_window = find_parent< window >( object_window );

		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		render.filled_rect( this->button_area , parent_window.theme.object_color );
		render.outlined_rect( this->button_area , parent_window.theme.border_color );

		renderer::pos text_pos = { this->button_area.x + ( this->button_area.w / 2 ) - ( text_size.w / 2 ),  this->button_area.y + ( this->button_area.h / 2 ) - ( text_size.h / 2 ) };
		render.text( text_pos, this->text, parent_window.font, helpers::clicking( this->button_area ) ? parent_window.theme.object_select_color : helpers::hovering( this->button_area ) ? parent_window.theme.main_color : parent_window.theme.text_color );

	}
}
