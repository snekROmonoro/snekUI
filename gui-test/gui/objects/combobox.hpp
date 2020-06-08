#ifndef COMBOBOX_HPP
#define COMBOBOX_HPP

#include "object.hpp"
namespace snekUI {

	class combobox : public object {
	public:
		combobox( std::string text , const std::vector< std::string >& items, const int& value = 0 ) {
			this->text = text;
			this->items = items;
			this->value = value;

			this->hovered_index = -1;
			this->opened = false;
		}

		~combobox( ) { }

		void think( );
		void draw( ) override;

		renderer::rect items_area;
		bool opened;
		renderer::rect area;
		int hovered_index;
		std::string text;
		std::vector< std::string > items;
		int value;
	};

}

#endif // !COMBOBOX_HPP