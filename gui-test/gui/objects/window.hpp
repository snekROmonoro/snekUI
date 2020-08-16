#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <array>
#include <functional>
#include <memory>
#include <vector>
#include "object.hpp"
namespace snekUI {
	class window : public object {
	public:
		window( LPD3DXFONT title_font , LPD3DXFONT font , std::string title , bool opened , int key , renderer::pos pos , renderer::dim dim ) {
			this->title_font = title_font;
			this->font = font;
			this->title = title;
			this->opened = opened;
			this->key = key;
			this->pos = pos;
			this->dim = dim;

			type = object_window;
		}

		window( LPD3DXFONT title_font , LPD3DXFONT font , std::string title , bool opened , int key , int x , int y , int w , int h ) {
			this->title_font = title_font;
			this->font = font;
			this->title = title;
			this->opened = opened;
			this->key = key;
			this->pos = renderer::pos { x , y };
			this->dim = renderer::dim { w , h };

			type = object_window;
		}

		window( std::string title , bool opened , int key , renderer::pos pos , renderer::dim dim ) {
			this->title = title;
			this->opened = opened;
			this->key = key;
			this->pos = pos;
			this->dim = dim;

			type = object_window;
		}

		window( std::string title , bool opened , int key , int x , int y , int w , int h ) {
			this->title = title;
			this->opened = opened;
			this->key = key;
			this->pos = renderer::pos { x , y };
			this->dim = renderer::dim { w , h };

			type = object_window;
		}

		~window( ) { }

		void update_font( LPD3DXFONT title_font , LPD3DXFONT font ) {
			this->title_font = title_font;
			this->font = font;
		}

		void think( );
		void draw( ) override;

		long __stdcall wndproc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam ) {
			if ( opened ) {

				/* we will do stuff here later. */
				switch ( msg ) {
				case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
				case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
				case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
				case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK: {
					int button = 0;
					if ( msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK ) { button = 0; }
					if ( msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK ) { button = 1; }
					if ( msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK ) { button = 2; }
					if ( msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK ) { button = ( GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ) ? 3 : 4; }
					mouse_down [ button ] = true;
					return true;
				}
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_MBUTTONUP:
				case WM_XBUTTONUP: {
					int button = 0;
					if ( msg == WM_LBUTTONUP ) { button = 0; }
					if ( msg == WM_RBUTTONUP ) { button = 1; }
					if ( msg == WM_MBUTTONUP ) { button = 2; }
					if ( msg == WM_XBUTTONUP ) { button = ( GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ) ? 3 : 4; }
					mouse_down [ button ] = false;
					return true;
				}
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
					if ( wparam < 256 )
						key_down [ wparam ] = true;
					return true;
				case WM_KEYUP:
				case WM_SYSKEYUP:
					if ( wparam < 256 )
						key_down [ wparam ] = false;
					return true;
				case WM_CHAR:
					if ( wparam > 0 && wparam < ( 1 << 16 ) && this->handle_keyboard )
						this->keyboard_handler_func( char( wparam ) );

					return true;
				}

				return true;
			}

			return false;
		}

		void* find_obj( const std::string& tab_name , const std::string& group_name , const std::string& obj_name , object_type obj_type );

		LPD3DXFONT title_font = nullptr;
		LPD3DXFONT font = nullptr;
		std::string title;
		bool opened = true;
		int key = -1;
		renderer::pos pos { 0, 0 };
		renderer::dim dim { 200, 200 };

		/* other vars */
		theme_t theme = theme_t( );
		renderer::pos cursor_pos { 0, 0 };
		std::array< bool , 5 > mouse_down { false };
		std::array< bool , 512 > key_down { false };

		/* overlays */
		std::function< void( ) > overlay_func;
		bool render_overlay = true;
		void draw_overlay( const std::function< void( ) >& overlay_renderer ) {
			overlay_func = overlay_renderer;
			render_overlay = true;
		}

		/* keyboard handler */
		std::function< void( char ) > keyboard_handler_func;
		bool handle_keyboard = true;
		void keyboard_handle( const std::function< void( char ) >& keyboard_handler ) {
			keyboard_handler_func = keyboard_handler;
			handle_keyboard = true;
		}

		/* objects */
		std::vector< std::shared_ptr< object > > objects;
		void add_object( std::shared_ptr< object > obj ) {
			obj->parent = this;
			objects.push_back( obj );
		}

	};
}

#endif // !WINDOW_HPP