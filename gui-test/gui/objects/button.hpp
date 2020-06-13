#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "object.hpp"
namespace snekUI {
	class button : public object {
	public:
		button( std::string text , std::function< void( ) > button_func ) {
			this->text = text;
			this->button_func = button_func;
			type = object_button;
		}

		~button( ) { }

		void think( );
		void draw( ) override;

		renderer::dim button_size = { 0, 20 }; /* we'll change width later in think( ) */
		renderer::rect button_area;
		std::string text;
		std::function< void( ) > button_func;
	};
}

#endif // !BUTTON_HPP