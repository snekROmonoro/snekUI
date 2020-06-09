#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../render/render.hpp"
#include "../theme/theme.hpp"
namespace snekUI {

	enum object_type {
		object_window,
		object_tab,
		object_group,
		object_checkbox,
		object_combobox,
		object_slider
	};

	class object {
	public:
		object* parent = nullptr;
		renderer::rect area;
		object_type type;

		template < typename type >
		type& find_parent( object_type type_to_find ) {
			auto parent_iter = this;

			try {
				while ( parent_iter->type != type_to_find )
					parent_iter = parent_iter->parent;
			}
			catch ( std::exception& ) {
			}

			return *static_cast< type* >( parent_iter );
		}

		virtual void draw( ) = 0;
		void draw_ex( );
	};

}

#endif // !OBJECT_HPP