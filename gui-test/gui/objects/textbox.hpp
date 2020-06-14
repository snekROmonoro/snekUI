#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "object.hpp"
namespace snekUI {
	class textbox : public object {
	public:
		textbox( const std::string& label , const std::string& buf , bool hide_input = false , size_t len_max = 32 ) {
			this->label = label;
			this->buf = buf;
			this->hide_input = hide_input;
			this->len_max = len_max;
			type = object_textbox;
		}

		~textbox( ) {}

		void handle_input( char key );
		void think( );
		void draw( ) override;

		bool opened = false;
		renderer::pos text_pos;
		renderer::rect textbox_rect;

		std::string label;
		std::string buf;
		bool hide_input;
		size_t len_max;
	};
}

#endif // !TEXTBOX_HPP