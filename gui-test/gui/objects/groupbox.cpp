#include "groupbox.hpp"
#include "window.hpp"
#include <algorithm>

namespace snekUI {

	void group::draw( ) {
		auto& parent_window = find_parent< window >( object_window );

		/* reset draw cursor pos */
		auto& cursor_pos = parent_window.cursor_pos;

		renderer::rect group_area = { cursor_pos.x , cursor_pos.y , area.w , area.h };
		render.filled_rect( group_area , parent_window.theme.background_group_color );

		renderer::dim title_text_size = render.text_size( this->title , parent_window.title_font );
		renderer::rect group_title_area = { group_area.x, group_area.y, group_area.w, parent_window.theme.titlebar_size < title_text_size.h ? title_text_size.h + 1 : parent_window.theme.titlebar_size };
		render.filled_rect( group_title_area , parent_window.theme.titlebar_color );

		render.text( { group_title_area.x + 6, group_title_area.y + ( group_title_area.h / 2 ) - ( title_text_size.h / 2 ) } , this->title , parent_window.title_font , parent_window.theme.titletext_color );

		/* move all objects inside group */
		cursor_pos.x += parent_window.theme.column_spacing;
		cursor_pos.y += group_title_area.h + 2;

		/* draw group objects */
		render.clip( group_area , [ & ] ( ) {
			std::for_each(
				objects.begin( ) ,
				objects.end( ) ,
				[ & ] ( std::shared_ptr< object >& child ) {
					child->area = renderer::rect { 0 , 0 , area.w - parent_window.theme.column_spacing * 2 , group_title_area.h };

					child->draw_ex( );
				}
			);
			} );
	}

}
