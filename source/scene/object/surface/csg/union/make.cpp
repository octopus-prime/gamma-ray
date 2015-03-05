/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/instance.hpp>
#include <scene/object/surface/csg/union/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/surface/csg/union/model.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

extern template class instance<union_::model>;

namespace union_ {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	const auto surface1 = surface::make(description->surface1);
	const auto surface2 = surface::make(description->surface2);

	BOOST_LOG_TRIVIAL(debug) << "Make surface union";

	const auto& box1 = surface1.get<1>();
	const auto& box2 = surface2.get<1>();

	const box_t box
	(
		{{
			std::min(box1.min_corner()[X], box2.min_corner()[X]),
			std::min(box1.min_corner()[Y], box2.min_corner()[Y]),
			std::min(box1.min_corner()[Z], box2.min_corner()[Z])
		}},
		{{
			std::max(box1.max_corner()[X], box2.max_corner()[X]),
			std::max(box1.max_corner()[Y], box2.max_corner()[Y]),
			std::max(box1.max_corner()[Z], box2.max_corner()[Z])
		}}
	);

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
