#pragma once
#include "../gui/gui.hpp"

namespace menu {
	long __stdcall wndproc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam );
	void create( );
	void destroy( );
	void reset( );
	void init( );
	void draw( );

	inline LPD3DXFONT menu_title_font;
	inline LPD3DXFONT menu_font;
	static std::shared_ptr< snekUI::window > window;
	inline bool initialized = false;

#define FIND( type, object, tab_name, group_name, object_name, object_type ) \
static auto& object = *( type* ) window->find_obj( std::string( tab_name ), std::string( group_name ), std::string( object_name ), object_type )

}
