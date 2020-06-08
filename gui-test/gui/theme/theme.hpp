#ifndef THEME_HPP
#define THEME_HPP

#include "../../other/color.hpp"
namespace snekUI {
	struct theme_t {
		theme_t( ) {
			draggable = true;
			drag_title = true;

			titlebar_color = color( 42 , 42 , 42 );
			titletext_color = color( 200 , 200 , 200 );
			titlebar_size = 20;

			background_color = color( 34 , 34 , 34 );
			border_color = color( 82 , 82 , 82 );

			object_color = color( 50 , 50 , 50 );
			object_select_color = color( 255 , 182 , 193 );

			main_color = color( 255 , 182 , 193 );
			text_color = color( 200 , 200 , 200 );
		}

		bool draggable;
		bool drag_title;

		color titlebar_color;
		color titletext_color;
		int titlebar_size;

		color background_color;
		color border_color;

		color object_color;
		color object_select_color;

		color main_color;
		color text_color;
	};
}

#endif // !THEME_HPP