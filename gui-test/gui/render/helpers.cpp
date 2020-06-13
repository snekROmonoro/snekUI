#include "helpers.hpp"

namespace snekUI {
	bool helpers::finished_input_frame = false;
	bool helpers::click_switch = false;

	bool helpers::hovering( int x , int y , int w , int h , bool override )
	{
		if ( ( !g_input && !override ) || finished_input_frame )
			return false;

		renderer::pos mpos;
		mouse_pos( mpos );

		if ( mpos.x > x && mpos.y > y && mpos.x < x + w && mpos.y < y + h )
			return true;

		return false;
	}

	bool helpers::hovering( renderer::pos p1 , renderer::pos p2 , bool override )
	{
		return hovering( p1.x , p1.y , p2.x , p2.y , override );
	}

	bool helpers::hovering( renderer::pos p , renderer::dim d , bool override )
	{
		return hovering( p.x , p.y , d.w , d.h , override );
	}

	bool helpers::hovering( renderer::rect r , bool override )
	{
		return hovering( r.x , r.y , r.w , r.h , override );
	}

	/* clicking */
	bool helpers::clicking( int x , int y , int w , int h , bool override )
	{
		if ( ( !g_input && !override ) || finished_input_frame )
			return false;

		if ( !click_switch && GetAsyncKeyState( VK_LBUTTON ) ) {
			if ( hovering( x , y , w , h ) ) {
				click_switch = true;
				finished_input_frame = true;
				return true;
			}
		}
		/* released key */
		else if ( click_switch && !GetAsyncKeyState( VK_LBUTTON ) ) {
			/* if you want, do something here? */
		}

		return false;
	}

	bool helpers::clicking( renderer::pos p1 , renderer::pos p2 , bool override )
	{
		return clicking( p1.x , p1.y , p2.x , p2.y , override );
	}

	bool helpers::clicking( renderer::pos p , renderer::dim d , bool override )
	{
		return clicking( p.x , p.y , d.w , d.h , override );
	}

	bool helpers::clicking( renderer::rect r , bool override )
	{
		return clicking( r.x , r.y , r.w , r.h , override );
	}

}
