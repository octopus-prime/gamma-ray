/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/instance.hpp>
#include <scene/object/surface/csg/intersection/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/surface/csg/intersection/model.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

extern template class instance<intersection::model>;

namespace intersection {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	const auto surface1 = surface::make(description->surface1);
	const auto surface2 = surface::make(description->surface2);

	const auto& box1 = surface1.get<1>();
	const auto& box2 = surface2.get<1>();

	box_t box;
	geo::intersection(box1, box2, box);

	BOOST_LOG_TRIVIAL(trace) << "Make intersection";
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
