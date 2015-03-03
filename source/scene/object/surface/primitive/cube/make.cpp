/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/cube/make.hpp>
#include <scene/object/surface/primitive/cube/model.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<cube::model>;

namespace cube {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	static const box_t box({{-1,-1,-1}}, {{+1,+1,+1}});

	BOOST_LOG_TRIVIAL(trace) << "Make cube";
	BOOST_LOG_TRIVIAL(trace) << "Box: " << geo::wkt(box.min_corner()) << ", " << geo::wkt(box.max_corner()) << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>(description->transformation),
		box
	);
}

}
}
}
}
}
}
