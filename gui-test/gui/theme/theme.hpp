#ifndef THEME_HPP
#define THEME_HPP

#include "../../other/color.hpp"
namespace snekUI {
	struct theme_t {
		theme_t( ) {
			animation_speed = 20 * 0.01; /* we transform the speed to decimals. */

			draggable = true;
			drag_title = true;

			column_spacing = 4;

			titlebar_color = color( 42 , 42 , 42 );
			titletext_color = color( 200 , 200 , 200 );
			titlebar_size = 20;

			background_color = color( 34 , 34 , 34 );
			background_group_color = color( 28 , 28 , 28 );
			border_color = color( 82 , 82 , 82 );

			object_color = color( 50 , 50 , 50 );
			object_select_color = color( 219 , 112 , 147 );

			main_color = color( 255 , 182 , 193 );
			text_color = color( 200 , 200 , 200 );
		}

		double animation_speed;

		bool draggable;
		bool drag_title;

		int column_spacing;

		color titlebar_color;
		color titletext_color;
		int titlebar_size;

		color background_color;
		color background_group_color;
		color border_color;

		color object_color;
		color object_select_color;

		color main_color;
		color text_color;
	};
}

#endif // !THEME_HPP