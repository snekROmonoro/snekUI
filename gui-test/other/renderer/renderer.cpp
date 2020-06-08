#include "renderer.hpp"
#include "../../main.hpp"

/* https://www.unknowncheats.me/forum/direct3d/60883-draw-drawprimtiveup-d3d9.html */

void renderer::start( LPDIRECT3DDEVICE9 dev ) {
	device = dev;

	/* just in-case */
	if ( device != dev ) {
		initialized = false;
		device = dev;
	}

	initialized = true;
}

void renderer::create_font( void** font , const std::string& family , int size , bool bold ) {
	if ( !initialized )
		return;

	ID3DXFont* d3d_font = nullptr;
	D3DXCreateFontA( device , size , 0 , bold ? FW_BOLD : FW_LIGHT , 1 , false , DEFAULT_CHARSET , OUT_DEFAULT_PRECIS , 0 , DEFAULT_PITCH | FF_DONTCARE , family.data( ) , &d3d_font );
	*font = d3d_font;
}

void renderer::create_font( void** font , const std::string& family , int size , int width ) {
	if ( !initialized )
		return;

	ID3DXFont* d3d_font = nullptr;
	D3DXCreateFontA( device , size , 0 , width , 1 , false , DEFAULT_CHARSET , OUT_DEFAULT_PRECIS , 0 , DEFAULT_PITCH | FF_DONTCARE , family.data( ) , &d3d_font );
	*font = d3d_font;
}

void renderer::screen_size( int& width , int& height ) {
	RECT screen_rect;
	GetWindowRect( GetDesktopWindow( ) , &screen_rect );

	width = screen_rect.right;
	height = screen_rect.bottom;
}

void renderer::text_size( void* font , const std::string& text , dim& dimentions ) {
	if ( !initialized )
		return;

	RECT rect = RECT( );
	reinterpret_cast< ID3DXFont* >( font )->DrawTextA( nullptr , text.data( ) , text.length( ) , &rect , DT_CALCRECT , D3DCOLOR_RGBA( 0 , 0 , 0 , 0 ) );
	dimentions = dim { rect.right - rect.left, rect.bottom - rect.top };
}

void renderer::rectangle( int x , int y , int width , int height , D3DCOLOR color ) {
	if ( !initialized )
		return;

	vtx_t vert [ 4 ] = {
		{ x - 0.5f, y - 0.5f, 0.0f, 1.0f, color },
		{ x + width - 0.5f, y - 0.5f, 0.0f, 1.0f, color },
		{ x - 0.5f, y + height - 0.5f, 0.0f, 1.0f, color },
		{ x + width - 0.5f, y + height - 0.5f, 0.0f, 1.0f, color }
	};

	device->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
	device->SetTexture( 0 , nullptr );
	device->SetPixelShader( nullptr );
	device->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	device->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 , &vert , sizeof( vtx_t ) );
}

void renderer::gradient( int x , int y , int width , int height , D3DCOLOR color1 , D3DCOLOR color2 , bool is_horizontal ) {
	const auto d3d_clr1 = color1;
	const auto d3d_clr2 = color2;
	std::uint32_t c1 , c2 , c3 , c4;

	if ( is_horizontal ) {
		c1 = d3d_clr1;
		c2 = d3d_clr2;
		c3 = d3d_clr1;
		c4 = d3d_clr2;
	}
	else {
		c1 = d3d_clr1;
		c2 = d3d_clr1;
		c3 = d3d_clr2;
		c4 = d3d_clr2;
	}


	vtx_t vert [ 4 ] = {
		{ x - 0.5f, y - 0.5f, 0.0f, 1.0f, c1 },
		{ x + width - 0.5f, y - 0.5f, 0.0f, 1.0f, c2 },
		{ x - 0.5f, y + height - 0.5f, 0.0f, 1.0f, c3 },
		{ x + width - 0.5f, y + height - 0.5f, 0.0f, 1.0f, c4 }
	};

	device->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
	device->SetTexture( 0 , nullptr );
	device->SetPixelShader( nullptr );
	device->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	device->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 , &vert , sizeof( vtx_t ) );
}

void renderer::outline( int x , int y , int width , int height , D3DCOLOR color ) {
	if ( !initialized )
		return;

	rectangle( x , y , width , 1 , color );
	rectangle( x , y + height , width , 1 , color );
	rectangle( x , y , 1 , height , color );
	rectangle( x + width , y , 1 , height + 1 , color );
}

void renderer::line( int x , int y , int x2 , int y2 , D3DCOLOR color ) {
	if ( !initialized )
		return;

	vtx_t vert [ 2 ] = {
	{ x - 0.5f, y - 0.5f, 0.0f, 1.0f, color },
	{ x2 - 0.5f, y2 - 0.5f, 0.0f, 1.0f, color }
	};

	device->SetRenderState( D3DRS_ALPHABLENDENABLE , true );
	device->SetTexture( 0 , nullptr );
	device->SetPixelShader( nullptr );
	device->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	device->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	device->DrawPrimitiveUP( D3DPT_LINELIST , 1 , &vert , sizeof( vtx_t ) );
}

void renderer::text( int x , int y , D3DCOLOR color , void* font , const std::string& text ) {
	if ( !initialized )
		return;

	RECT rect;
	SetRect( &rect , x , y , x , y );
	reinterpret_cast< ID3DXFont* >( font )->DrawTextA( nullptr , text.data( ) , text.length( ) , &rect , DT_LEFT | DT_NOCLIP , color );
}

void renderer::mouse_pos( pos& position ) {
	RECT rect;
	GetWindowRect( GetActiveWindow( ) , &rect );

	POINT Pos;
	GetCursorPos( &Pos );

	position.x = Pos.x - rect.left;
	position.y = Pos.y - rect.top;
}
