#include "label.hpp"
#include "window.hpp"

namespace snekUI {
	void label::think( )
	{
		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		/* get normal label area */
		this->label_area = { parent_window.cursor_pos.x, parent_window.cursor_pos.y, 0 /* we will get width later. */, text_size.h + 1 };

		/* get label area width */
#ifdef LABEL_WIDTH_AUTO
		this->label_area.w = text_size.w + 1;
#else
		this->label_area.w = this->area.w;
#endif

		/* set new cursor pos Y */
		parent_window.cursor_pos.y += this->label_area.h + parent_window.theme.object_spacing;
	}

	void label::draw( )
	{
		think( );

		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		render.filled_rect( this->label_area , parent_window.theme.object_color );
		render.outlined_rect( this->label_area , parent_window.theme.border_color );

		const renderer::pos text_pos = { this->label_area.x + ( this->label_area.w / 2 ) - ( text_size.w / 2 ), this->label_area.y + ( this->label_area.h / 2 ) - ( text_size.h / 2 ) };
		render.text( text_pos , this->text , parent_window.font , helpers::clicking( this->label_area ) ? parent_window.theme.object_select_color : helpers::hovering( this->label_area ) ? parent_window.theme.main_color : parent_window.theme.text_color );
	}
}
