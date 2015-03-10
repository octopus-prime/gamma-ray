/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/superellipsoid/make.hpp>
#include <scene/object/surface/primitive/superellipsoid/model.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<superellipsoid::model>;

namespace superellipsoid {

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface superellipsoid";

	const box_t box = transform(description->transformation, box_t({{-1,-1,-1}}, {{+1,+1,+1}}));

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>(description->transformation, description->e, description->n),
		box,
		11
	);
}

}
}
}
}
}
}
