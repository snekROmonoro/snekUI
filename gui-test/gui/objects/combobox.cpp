#include <algorithm>
#include "combobox.hpp"
#include "window.hpp"

/* snek ceo of std::max_element */

namespace snekUI {

	void combobox::think( )
	{

		auto& parent_window = find_parent< window >( object_window );

		/* get text-size of main text */
		renderer::dim text_size = render.text_size( this->text , parent_window.font );

		/* get text-size of every item */
		std::vector< int > item_text_sizes_w;
		std::vector< int > item_text_sizes_h;
		for ( const auto& item : this->items ) {
			auto item_text_size = render.text_size( item , parent_window.font );

			item_text_sizes_w.push_back( item_text_size.w );
			item_text_sizes_h.push_back( item_text_size.h );
		}

		/* get max item text size */
		renderer::dim max_item_size = { *std::max_element( item_text_sizes_w.begin( ), item_text_sizes_w.end( ) ), *std::max_element( item_text_sizes_h.begin( ), item_text_sizes_h.end( ) ) };

		/* get max item label capacity */
		int max_item_label_index = 0;
		std::vector< std::pair< int , int > > max_item_label_capacity;
		for ( const auto& item : this->items ) {
			max_item_label_capacity.push_back( { max_item_label_index, item.capacity( ) } );
			max_item_label_index++;
		}

		/* get combo_area to draw main rectangle ( which we gonna use to open items combo_area )*/
#ifdef COMBOBOX_WIDTH_AUTO /* using text-size, not max child width. */
		std::string max_label = this->items [ std::pair< int , int >( *std::max_element( max_item_label_capacity.begin( ) , max_item_label_capacity.end( ) ) ).first ];
		renderer::dim label_text_size = render.text_size( this->text + " - " + max_label , parent_window.font );
		this->combo_area = renderer::rect { parent_window.cursor_pos.x, parent_window.cursor_pos.y, label_text_size.w + max_item_size.w, text_size.h };
#else
		this->combo_area = renderer::rect { parent_window.cursor_pos.x, parent_window.cursor_pos.y, this->area.w, text_size.h };
#endif

		/* get items combo_area */
		/* INFO: we do '+ this->items.size( )' because we are going to draw 1 pixel down every item */
		this->items_area = renderer::rect { parent_window.cursor_pos.x, parent_window.cursor_pos.y + this->combo_area.h + 2, this->combo_area.w , max_item_size.h + int( this->items.size( ) ) };

		/* handle opening items selector */
		if ( helpers::clicking( this->combo_area , this->opened ) ) {
			this->opened = !this->opened;
			helpers::g_input = !opened;

			/* re-enable input if should */
			if ( !this->opened ) {
				helpers::finished_input_frame = true;
				helpers::g_input = true;
			}
		}

		/* handle item selector */
		if ( this->opened ) {
			this->hovered_index = -1;
			int index = 0;

			renderer::rect next_item_area = this->items_area;
			for ( const auto& item : this->items ) {
				if ( helpers::clicking( next_item_area , true ) ) {
					this->value = index;
				}
				else if ( helpers::hovering( next_item_area , true ) ) {
					hovered_index = index;
				}

				next_item_area.y += render.text_size( item , parent_window.font ).h;
				index++;
			}

#ifdef SNEKUI_NO_INPUT
			parent_window.cursor_pos.y += next_item_area.y - this->items_area.y + parent_window.theme.object_spacing;
#endif
		}

		/* set new cursor pos Y */
		parent_window.cursor_pos.y += this->combo_area.h + parent_window.theme.object_spacing;

	}

	void combobox::draw( )
	{
		think( );

		auto& parent_window = find_parent< window >( object_window );

		/* main rectangle, where we let the people know they can open this combobox + their selection */
		render.filled_rect( this->combo_area , parent_window.theme.object_color );
		render.outlined_rect( this->combo_area , parent_window.theme.border_color );

		/* label with their selection ( and ofcourse combobox title ) */
		std::string label_text = this->text + " - " + this->items [ this->value ];
		renderer::dim label_text_size = render.text_size( label_text , parent_window.font );
		render.text( { this->combo_area.x + ( this->combo_area.w / 2 ) - ( label_text_size.w / 2 ),  this->combo_area.y + ( this->combo_area.h / 2 ) - ( label_text_size.h / 2 ) } , label_text , parent_window.font , helpers::hovering( this->combo_area , this->opened ) ? parent_window.theme.main_color : parent_window.theme.text_color );

		/* we'll need this later */
		static renderer::rect best_items_area = { 0 , 0 };

		/* draw overlay for items selector */
		if ( this->opened ) {
			/* draw it as an overlay, because we most probably will have more objects after this. */
			parent_window.draw_overlay( [ = ] ( ) {
				std::vector< std::function< void( ) > > draw_functions; /* we gonna need this later */
				renderer::rect max_item_area = this->items_area;

				int index = 0;
				for ( const auto& item : this->items ) {

					renderer::dim item_text_size = render.text_size( item , parent_window.font );
					renderer::pos item_draw_pos = { this->items_area.x, this->items_area.y + this->items_area.h * ( index + 1 ) };
					draw_functions.push_back( [ = ] ( ) {
						render.text( { item_draw_pos.x + ( this->items_area.w / 2 ) - ( item_text_size.w / 2 ) , item_draw_pos.y - ( this->items_area.h / 2 ) - ( item_text_size.h / 2 ) } , item , parent_window.font , this->value == index ? parent_window.theme.object_select_color : this->hovered_index == index ? parent_window.theme.main_color : parent_window.theme.text_color );

						/* draw a line on the bottom ( not for the last index ) */
						if ( index != this->items.size( ) - 1 ) {
							render.filled_rect( item_draw_pos , { this->combo_area.w, 1 } , parent_window.theme.border_color );
						}

						} );

					max_item_area.h += item_text_size.h * index;
					index++;
				}

				render.filled_rect( max_item_area , parent_window.theme.object_color );
				render.outlined_rect( max_item_area , parent_window.theme.border_color );
				for ( auto& draw_function : draw_functions ) {
					draw_function( );
				}

				best_items_area = max_item_area;
				} );

			/* handle clicking outside combo-box */
			renderer::rect max_combo_area = this->combo_area; max_combo_area.h += 2; /* because if we click between items/combo area */
			if ( !helpers::click_switch /* after clicking to open */ && GetAsyncKeyState( VK_LBUTTON ) && !helpers::hovering( best_items_area , true ) && !helpers::hovering( max_combo_area , true ) ) {
				/* re-enable input */
				helpers::finished_input_frame = true;
				helpers::g_input = true;

				/* close overlay */
				this->opened = false;
			}

		}
	}
}
