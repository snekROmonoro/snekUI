#ifndef HELPERS_HPP
#define HELPERS_HPP

/* especially used to handle input. */
#include "render.hpp"

namespace snekUI {

	class dividers {
	public:
		std::vector< int > columns_per_row;
	};

	namespace helpers {
		static bool g_input = true;
		extern bool finished_input_frame;
		extern bool click_switch;

		bool hovering( int x , int y , int w , int h , bool override = false );
		bool hovering( renderer::pos p1 , renderer::pos p2 , bool override = false );
		bool hovering( renderer::pos p , renderer::dim d , bool override = false );
		bool hovering( renderer::rect r , bool override = false );

		bool clicking( int x , int y , int w , int h , bool override = false );
		bool clicking( renderer::pos p1 , renderer::pos p2 , bool override = false );
		bool clicking( renderer::pos p , renderer::dim d , bool override = false );
		bool clicking( renderer::rect r , bool override = false );
	}

}

#endif // !HELPERS_HPP