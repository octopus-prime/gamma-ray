/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/difference/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/surface/csg/difference/model.hpp>
#include <scene/object/surface/csg/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

extern template class instance<difference::model>;

namespace difference {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	const auto surface1 = surface::make(description->surface1);
	const auto surface2 = surface::make(description->surface2);

	BOOST_LOG_TRIVIAL(debug) << "Make surface difference";

	const auto& box = surface1.get<1>();

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		csg::make<model>
		(
			surface1.get<0>(),
			surface2.get<0>()
		),
		box
	);
}

}
}
}
}
}
}
