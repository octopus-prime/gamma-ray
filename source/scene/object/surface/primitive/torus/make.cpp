/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/torus/make.hpp>
#include <scene/object/surface/primitive/torus/model.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<torus::model>;

namespace torus {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	const float radius = description->major + description->minor;
	const vector3_t bound {{radius, radius, description->minor}};

	const box_t box = transform(description->transformation, box_t(-bound, +bound));

	BOOST_LOG_TRIVIAL(trace) << "Make torus";
	BOOST_LOG_TRIVIAL(trace) << "Box: " << geo::wkt(box.min_corner()) << ", " << geo::wkt(box.max_corner()) << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>
		(
			description->transformation,
			description->major,
			description->minor
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
