#pragma once
#include <d3d9.h>
#include <d3dx9.h>

struct color {
	color( ) : r( 255 ) , g( 255 ) , b( 255 ) , a( 255 ) { } /* default white */
	color( int r , int g , int b , int a = 255 ) : r( r ) , g( g ) , b( b ) , a( a ) { }
	~color( ) { }

	D3DCOLOR to_d3d( ) {
		return D3DCOLOR_RGBA( r , g , b , a );
	}

	color alpha( int a ) {
		return color( r , g , b , a );
	}

	color black( int a = 255 ) {
		return color( 0 , 0 , 0 , a );
	}

	color red( int a = 255 ) {
		return color( 255 , 0 , 0 , a );
	}

	color green( int a = 255 ) {
		return color( 0 , 255 , 0 , a );
	}

	color blue( int a = 255 ) {
		return color( 0 , 0 , 255 , a );
	}

	color pink( int a = 255 ) {
		return color( 255 , 192 , 203 , a );
	}

	color light_pink( int a = 255 ) {
		return color( 255 , 182 , 193 );
	}

	color hot_pink( int a = 255 ) {
		return color( 255 , 105 , 180 );
	}

	int r , g , b , a;
};
