#ifndef LABEL_HPP
#define LABEL_HPP

#include "object.hpp"
namespace snekUI {
	class label : public object {
	public:
		label( std::string text ) {
			this->text = text;
			type = object_label;
		}

		~label( ) { }

		void think( );
		void draw( ) override;

		renderer::rect label_area;
		std::string text;
		std::function< void( ) > button_func;
	};
}

#endif // !LABEL_HPP