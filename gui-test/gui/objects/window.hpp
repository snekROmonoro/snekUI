#ifndef WINDOW_HPP
#define WINDOW_HPP

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

		~window( ) { }

		void think( );
		void draw( ) override;

		long __stdcall wndproc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam ) {
			if ( opened ) {

				/* we will do stuff here later. */

				return true;
			}

			return false;
		}

		void* find_obj( std::string obj_name , object_type obj_type );

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

		/* objects */
		std::vector< std::shared_ptr< object > > objects;
		void add_object( std::shared_ptr< object > obj ) {
			obj->parent = this;
			objects.push_back( obj );
		}

	};
}

#endif // !WINDOW_HPP