/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/isosurface/make.hpp>
#include <scene/object/surface/primitive/isosurface/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<isosurface::model>;

namespace isosurface {

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface isosurface";

	box_t box({{-3,-3,-3}}, {{+3,+3,+3}});	// todo

	function_t function = description->function;

	rtree_t rtree;

	const vector3_t delta = (box.max_corner() - box.min_corner()) / 100.f;
	for (std::size_t x = 0; x < 100; ++x)
		for (std::size_t y = 0; y < 100; ++y)
			for (std::size_t z = 0; z < 100; ++z)
			{
				const vector3_t min
				{{
					box.min_corner()[X] + delta[X] * x,
					box.min_corner()[Y] + delta[Y] * y,
					box.min_corner()[Z] + delta[Z] * z
				}};
				const vector3_t max = min + delta;

				std::size_t count = 0;
				count += function({{min[X], min[Y], min[Z]}}) >= 0;
				count += function({{min[X], min[Y], max[Z]}}) >= 0;
				count += function({{min[X], max[Y], min[Z]}}) >= 0;
				count += function({{min[X], max[Y], max[Z]}}) >= 0;
				count += function({{max[X], min[Y], min[Z]}}) >= 0;
				count += function({{max[X], min[Y], max[Z]}}) >= 0;
				count += function({{max[X], max[Y], min[Z]}}) >= 0;
				count += function({{max[X], max[Y], max[Z]}}) >= 0;

				if (count != 0 && count != 8)
					rtree.insert(value_t(box_t(min, max), boost::none));
			}

	box = transform
	(
		description->transformation,
		box_t// TODO: puke =>
		(
			{{
				geo::get<X>(rtree.bounds().min_corner()),
				geo::get<Y>(rtree.bounds().min_corner()),
				geo::get<Z>(rtree.bounds().min_corner())
			}},
			{{
				geo::get<X>(rtree.bounds().max_corner()),
				geo::get<Y>(rtree.bounds().max_corner()),
				geo::get<Z>(rtree.bounds().max_corner())
			}}
		)
	);

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << ", N = " << rtree.size() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>
		(
			description->transformation,
			std::move(function),
			std::move(rtree)
		),
		box,
		2 * 10	// todo
	);
}

}
}
}
}
}
}
