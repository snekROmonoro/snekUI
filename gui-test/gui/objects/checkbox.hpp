#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "object.hpp"
namespace snekUI {

	class checkbox : public object {
	public:
		checkbox( std::string text , const bool& value = false ) {
			this->text = text;
			this->value = value;
		}

		~checkbox( ) { }

		void think( );
		void draw( ) override;

		renderer::dim check_size = { 20, 20 };
		renderer::rect check_area;
		renderer::rect area;
		std::string text;
		bool value;
	};

}

#endif // !CHECKBOX_HPP