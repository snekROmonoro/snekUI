#ifndef GROUPBOX_HPP
#define GROUPBOX_HPP

#include "object.hpp"
namespace snekUI {

	class group : public object {
	public:
		std::vector< std::shared_ptr< object > > objects;
		std::string title;

		group( const std::string& title ) {
			this->title = title;
			type = object_group;
		}

		~group( ) {}

		void add_element( const std::shared_ptr< object >& new_obj ) {
			new_obj->parent = this;
			objects.push_back( new_obj );
		}

		void draw( ) override;
	};

}

#endif // !GROUPBOX_HPP