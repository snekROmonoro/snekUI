#ifndef RENDER_HPP
#define RENDER_HPP

#include <functional>
#include <vector>
#include "../../other/color.hpp"
#include "../../other/renderer/renderer.hpp"
#include "helpers.hpp"
namespace snekUI {

	struct render {

		virtual void clip( renderer::rect area , std::function< void( ) > func ) {
			RECT backup_scissor_rect;
			renderer::device->GetScissorRect( &backup_scissor_rect );

			RECT rect { area.x - 0.5f, area.y - 0.5f, area.x + area.w - 0.5f, area.y + area.h - 0.5f };
			renderer::device->SetRenderState( D3DRS_SCISSORTESTENABLE , true );
			renderer::device->SetScissorRect( &rect );

			func( );

			renderer::device->SetScissorRect( &backup_scissor_rect );
			renderer::device->SetRenderState( D3DRS_SCISSORTESTENABLE , false );
		}

		virtual void filled_rect( int x , int y , int width , int height , color color ) {
			renderer::rectangle( x , y , width , height , color.to_d3d( ) );
		}

		virtual void filled_rect( renderer::rect r , color color ) {
			filled_rect( r.x , r.y , r.w , r.h , color );
		}

		virtual void filled_rect( renderer::pos p , renderer::dim d , color color ) {
			filled_rect( p.x , p.y , d.w , d.h , color );
		}

		virtual void outlined_rect( int x , int y , int width , int height , color color ) {
			renderer::outline( x , y , width , height , color.to_d3d( ) );
		}

		virtual void outlined_rect( renderer::rect r , color color ) {
			outlined_rect( r.x , r.y , r.w , r.h , color );
		}

		virtual void outlined_rect( renderer::pos p , renderer::dim d , color color ) {
			outlined_rect( p.x , p.y , d.w , d.h , color );
		}

		virtual renderer::dim text_size( std::string text , LPD3DXFONT font ) {
			renderer::dim size;
			renderer::text_size( ( void* ) font , text , size );
			return size;
		}

		virtual void text( int x , int y , std::string t , LPD3DXFONT f , color c ) {
			renderer::text( x , y , c.to_d3d( ) , ( void* ) f , t );
		}

		virtual void text( renderer::pos p , std::string t , LPD3DXFONT f , color c ) {
			text( p.x , p.y , t , f , c );
		}

		virtual void screen_size( int& w , int& h ) {
			renderer::screen_size( w , h );
		}

		virtual void screen_size( renderer::dim& s ) {
			screen_size( s.w , s.h );
		}

		virtual void mouse_pos( int& x , int& y ) {
			renderer::pos p;
			renderer::mouse_pos( p );
			x = p.x;
			y = p.y;
		}

		virtual void mouse_pos( renderer::pos& p ) {
			mouse_pos( p.x , p.y );
		}

	}static render;
}

#endif // !RENDER_HPP