/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/sphere/make.hpp>
#include <scene/object/surface/primitive/sphere/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<sphere::model>;

namespace sphere {

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface sphere";

	matrix44_t transformation = identity<4>();
	if (description->radius)
		transformation *= rt::scale({{*description->radius, *description->radius, *description->radius}});
	if (description->origin)
		transformation *= rt::translate(*description->origin);

	const box_t box = transform(transformation * description->transformation, box_t({{-1,-1,-1}}, {{+1,+1,+1}}));

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>(transformation * description->transformation),
		box,
		2
	);
}

}
}
}
}
}
}
