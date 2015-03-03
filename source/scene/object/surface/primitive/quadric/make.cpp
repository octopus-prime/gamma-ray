/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/quadric/make.hpp>
#include <scene/object/surface/primitive/quadric/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<quadric::model>;

namespace quadric {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	matrix44_t m = 0.5f * matrix44_t
	{{
		{{2 * description->square[0],	description->mixed[0],		description->mixed[1],		description->absolute[0]}},
		{{description->mixed[0],		2 * description->square[1],	description->mixed[2], 		description->absolute[1]}},
		{{description->mixed[1],		description->mixed[2],		2 * description->square[2],	description->absolute[2]}},
		{{description->absolute[0],		description->absolute[1],	description->absolute[2],	2 * description->constant}}
	}};

//	matrix44_t m = description->transformation * mx * transpose(description->transformation);

	const float dx = std::sqrt(m[W][X] * m[W][X] - m[W][W] * m[X][X]) + std::numeric_limits<float>::min();
	const float x0 = m[W][W] / (m[W][X] + dx);
	const float x1 = m[W][W] / (m[W][X] - dx);

	const float dy = std::sqrt(m[W][Y] * m[W][Y] - m[W][W] * m[Y][Y]) + std::numeric_limits<float>::min();
	const float y0 = m[W][W] / (m[W][Y] + dy);
	const float y1 = m[W][W] / (m[W][Y] - dy);

	const float dz = std::sqrt(m[W][Z] * m[W][Z] - m[W][W] * m[Z][Z]) + std::numeric_limits<float>::min();
	const float z0 = m[W][W] / (m[W][Z] + dz);
	const float z1 = m[W][W] / (m[W][Z] - dz);

	const box_t box({{x0, y0, z0}}, {{x1, y1, z1}});

	BOOST_LOG_TRIVIAL(trace) << "Make quadric";
	BOOST_LOG_TRIVIAL(trace) << "Box: " << geo::wkt(box.min_corner()) << ", " << geo::wkt(box.max_corner()) << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>(description->transformation, std::move(m)),
		box
	);
}

}
}
}
}
}
}
