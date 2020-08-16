#include <chrono>
#include "textbox.hpp"
#include "window.hpp"

/* big credits to: sesame.one */

namespace snekUI {

	void textbox::handle_input( char key ) {
		auto& parent_window = find_parent< window >( object_window );

		switch ( key ) {
		case VK_TAB:
		case VK_ESCAPE:
		case VK_RETURN: /* anything that will toggle selection */
			this->opened = false;
			helpers::g_input = true;
			parent_window.handle_keyboard = false;
			return;
		case VK_BACK: /* backspace */
			if ( !this->buf.empty( ) )
				this->buf.pop_back( );
			return;
		default: /* handle text input */ {
			/* keyboard shortcuts */
			if ( ( parent_window.key_down [ VK_CONTROL ] || parent_window.key_down [ VK_LCONTROL ] ) && parent_window.key_down [ 'V' ] ) {
				HGLOBAL global = nullptr;

				OpenClipboard( nullptr );

				if ( global = GetClipboardData( CF_TEXT ) ) {
					auto wglobal = ( char* ) GlobalLock( global );

					if ( wglobal && GlobalSize( global ) ) {
						this->buf += std::string( wglobal + 0 , wglobal + GlobalSize( global ) / 2 + 1 );

						if ( this->buf.length( ) >= len_max )
							this->buf.erase( len_max );
					}

					GlobalUnlock( global );
				}

				CloseClipboard( );

				return;
			}

			if ( this->buf.length( ) < len_max )
				this->buf.push_back( key );
		} return;
		}
	}

	void textbox::think( ) {
		auto& parent_window = find_parent< window >( object_window );

		/* get text-size */
		renderer::dim text_size = render.text_size( this->label , parent_window.font );

		/* get text position we're going to use to draw label text! */
		this->text_pos = parent_window.cursor_pos;

		/* get textbox rect which we're going to use to draw! */
		this->textbox_rect = { parent_window.cursor_pos.x, parent_window.cursor_pos.y + text_size.h + 2, this->area.w, render.text_size( "*" , parent_window.font ).h /* can't think of one */ };

		if ( helpers::clicking( this->textbox_rect , this->opened ) && !this->opened ) {
			this->opened = !this->opened;
			helpers::g_input = false;
		}

		/* check if clicking outside of area (disable input) */
		if ( this->opened && !helpers::click_switch /* after clicking to open */ && GetAsyncKeyState( VK_LBUTTON ) && !helpers::hovering( this->textbox_rect , true ) ) {
			/* re-enable input */
			helpers::finished_input_frame = true;
			helpers::g_input = true;

			/* close */
			parent_window.handle_keyboard = false;
			this->opened = false;
		}

		if ( this->opened ) {
			/* set input handler */
			parent_window.keyboard_handle( [ = ] ( char key ) { handle_input( key ); } );
		}

		/* set new cursor pos Y */
		parent_window.cursor_pos.y += text_size.h + this->textbox_rect.h + ( parent_window.theme.object_spacing * 2 );
	}

	void textbox::draw( ) {
		think( );

		auto& parent_window = find_parent< window >( object_window );

		renderer::dim text_size = render.text_size( this->label , parent_window.font );
		auto input_text = this->buf;

		/* replace visible characters with stars (*)*/
		if ( this->hide_input ) {
			input_text.erase( );
			std::for_each( this->buf.begin( ) , this->buf.end( ) , [ & ] ( wchar_t character ) { input_text += std::string( "*" ); } );
		}

		renderer::dim input_text_size = render.text_size( input_text , parent_window.font );

		render.text( this->text_pos , this->label , parent_window.font , parent_window.theme.text_color );
		render.filled_rect( this->textbox_rect , parent_window.theme.object_color );
		render.outlined_rect( this->textbox_rect , parent_window.theme.border_color );

		const renderer::pos input_text_pos = { this->textbox_rect.x + parent_window.theme.object_spacing , this->textbox_rect.y + ( this->textbox_rect.h / 2 ) - ( input_text_size.h / 2 ) };
		render.text( input_text_pos , input_text , parent_window.font , parent_window.theme.text_color );

		/* input cursor */
		if ( this->opened ) {
			double time = static_cast< double > ( std::chrono::duration_cast< std::chrono::milliseconds > ( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( ) ) / 1000.0;
			render.filled_rect( { input_text_pos.x + input_text_size.w + 2, this->textbox_rect.y + 2, 2, render.text_size( "*" , parent_window.font ).h - 4 } , parent_window.theme.text_color.alpha( static_cast< int >( std::sin( time * 2.0 * 3.141f ) * 127.5 + 127.5 ) ) );
		}

	}
}
