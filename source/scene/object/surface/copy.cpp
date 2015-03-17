/*
 * copy.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/copy.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {

class copy_visitor
:
	public boost::static_visitor<description_t>
{
public:
	template <typename CSG>
	result_type operator()(const csg::description_t<CSG>& description) const
	{
		return boost::make_shared<csg::basic_description_t<CSG>>
		(
			boost::apply_visitor(*this, description->surface1),
			boost::apply_visitor(*this, description->surface2)
		);
	}

	template <typename Primitive>
	result_type operator()(const primitive::description_t<Primitive>& description) const
	{
		return boost::make_shared<primitive::basic_description_t<Primitive>>(*description);
	}
};

description_t
copy(const description_t& description)
{
	return boost::apply_visitor(copy_visitor(), description);
}

}
}
}
}
