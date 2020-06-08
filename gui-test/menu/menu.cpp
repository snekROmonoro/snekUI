#include "menu.hpp"

long __stdcall menu::wndproc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam )
{
	if ( !initialized )
		return false;

	return window->wndproc( hwnd , msg , wparam , lparam );
}

void menu::create( )
{
	/* we will use this function to create our fonts */
	renderer::create_font( ( void** ) &menu_title_font , "Tahoma" , 16 , FW_BOLD | FW_LIGHT );
	renderer::create_font( ( void** ) &menu_font , "Verdana" , 16 , FW_NORMAL );
}

void menu::destroy( )
{
	if ( menu_title_font ) menu_title_font->Release( );
	if ( menu_font) menu_font->Release( );
}

void menu::reset( )
{
	create( );
}

void menu::init( ) {
	/* create ( not only once ) */
	create( );

	if ( initialized )
		return;

	/* get screen size */
	renderer::dim screen_size;
	renderer::screen_size( screen_size.w , screen_size.h );

	/* get dimensions and positions for following windows. */
	renderer::dim dim { 600, 400 };
	renderer::pos pos { ( screen_size.w / 2 ) - ( dim.w / 2 ), ( screen_size.h / 2 ) - ( dim.h / 2 ) };

	/* create window */
	window = std::make_shared< snekUI::window >( menu_title_font , menu_font , "snekUI Example Window" , true , VK_INSERT , pos , dim );

	/* add objects to menu */
	window->add_object( std::make_shared< snekUI::checkbox >( "test" ) );
	window->add_object( std::make_shared< snekUI::combobox >( "test_combo" , std::vector< std::string >{ "first" , "second" } ) );
	window->add_object( std::make_shared< snekUI::checkbox >( "test2" ) );
	window->add_object( std::make_shared< snekUI::slider >( "test_slider", 0, 100 ) );
	window->add_object( std::make_shared< snekUI::checkbox >( "test3" ) );

	initialized = true;
}

void menu::draw( )
{
	/* initialize */
	init( ); /* in-case we already initialized, it won't re-init, because 'initialized' bool will be true. */

	/* check if we really initialized */
	if ( !initialized ) return;

	window->draw( );
}
