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

	auto new_tab = std::make_shared< snekUI::tab >( "tab" ); {

		auto new_group = std::make_shared< snekUI::group >( "group1" ); {
			new_group->add_element( std::make_shared< snekUI::checkbox >( "test" ) );
			new_group->add_element( std::make_shared< snekUI::combobox >( "test_combo" , std::vector< std::string >{ "first" , "second" } ) );
			new_group->add_element( std::make_shared< snekUI::checkbox >( "test2" ) );
			new_group->add_element( std::make_shared< snekUI::slider >( "test_slider" , 0 , 100 ) );
			new_group->add_element( std::make_shared< snekUI::checkbox >( "test3" ) );
		} new_tab->add_element( new_group );

		new_tab->add_columns( 1 );

		auto new_group2 = std::make_shared< snekUI::group >( "group2" ); {
			new_group2->add_element( std::make_shared< snekUI::checkbox >( "test2" ) );
		} new_tab->add_element( new_group2 );

		new_tab->add_columns( 1 );

	} window->add_object( new_tab );

	auto new_tab2 = std::make_shared< snekUI::tab >( "tab2" ); {

		auto new_group = std::make_shared< snekUI::group >( "group1" ); {
			new_group->add_element( std::make_shared< snekUI::checkbox >( "test" ) );
		} new_tab2->add_element( new_group );

		auto new_group2 = std::make_shared< snekUI::group >( "group2" ); {
			new_group2->add_element( std::make_shared< snekUI::checkbox >( "test2" ) );
		} new_tab2->add_element( new_group2 );

		new_tab2->add_columns( 2 );

	} window->add_object( new_tab2 );

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
