#ifndef TAB_HPP
#define TAB_HPP

#include "object.hpp"
#include <memory>
namespace snekUI {

	class tab : public object {
		dividers divider;

	public:
		std::vector< std::shared_ptr< object > > objects;
		bool selected = false;
		std::string title;

		tab( const std::string& title ) {
			this->title = title;
			type = object_tab;
		}
		~tab( ) {}

		void add_columns( int columns ) {
			divider.columns_per_row.push_back( columns );
		}

		void add_element( const std::shared_ptr< object >& new_obj ) {
			new_obj->parent = this;
			objects.push_back( new_obj );
		}

		void draw( ) override;
	};
}

#endif // !TAB_HPP
