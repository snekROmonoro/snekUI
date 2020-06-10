#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "object.hpp"
namespace snekUI {

	class slider : public object {
	public:
		slider( std::string text , const int& min , const int& max , const int& value = INT_MAX ) {
			this->text = text;
			this->min = min;
			this->max = max;
			this->value = value == INT_MAX ? min : value;

			type = object_slider;
		}

		~slider( ) { }

		void think( );
		void draw( ) override;

		renderer::dim slider_size { 0, 15 }; /* Change this if you want ofcourse. ( Changing width won't change anything ) */
		renderer::pos text_pos;
		renderer::rect slider_rect;
		std::string text;
		int min;
		int max;
		int value;
	};

}

#endif // !SLIDER_HPP