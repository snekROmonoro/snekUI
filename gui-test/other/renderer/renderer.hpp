#pragma once
#include <string>
#include <d3d9.h>
#include <d3dx9.h>

namespace renderer {
	void start( LPDIRECT3DDEVICE9 dev );

	struct vtx_t {
		float x , y , z , rhw;
		DWORD color;
	};

	struct pos {
		int x , y;
	};

	struct dim {
		int w , h;
	};

	struct rect {
		int x , y , w , h;

		bool operator==( const rect& v ) {
			return ( this->x == v.x
				&& this->y == v.y
				&& this->w == v.w
				&& this->h == v.h );
		}

		bool operator!=( const rect& v ) {
			return ( this->x == v.x
				|| this->y == v.y
				|| this->w == v.w
				|| this->h == v.h );
		}
	};

	void create_font( void** font , const std::string& family , int size , bool bold );
	void create_font( void** font , const std::string& family , int size , int width );
	void screen_size( int& width , int& height );
	void text_size( void* font , const std::string& text , dim& dimentions );
	void rectangle( int x , int y , int width , int height , D3DCOLOR color );
	void gradient( int x , int y , int width , int height , D3DCOLOR color1 , D3DCOLOR color2 , bool is_horizontal );
	void outline( int x , int y , int width , int height , D3DCOLOR color );
	void line( int x , int y , int x2 , int y2 , D3DCOLOR color );
	void text( int x , int y , D3DCOLOR color , void* font , const std::string& text );
	void mouse_pos( pos& position );

	inline LPDIRECT3DDEVICE9 device;
	inline bool initialized = false;
}
