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

static box_t
box(const matrix44_t& m)
{
	vector3_t min, max;

	for (std::size_t i = 0; i < 3; ++i)
	{
		const float d = std::sqrt(m[W][i] * m[W][i] - m[W][W] * m[i][i]) + std::numeric_limits<float>::min();
		min[i] = m[W][W] / (m[W][i] + d);
		max[i] = m[W][W] / (m[W][i] - d);
	}

	return box_t(min, max);
}

static box_t
foo(const matrix44_t& t, const matrix44_t& m)
{
	return box(t * invert(m) * transpose(t));
}

static box_t
bar(const matrix44_t& t, const matrix44_t& m)
{
	return transform(t, box(m));
}

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface quadric";

	matrix44_t m = 0.5f * matrix44_t
	{{
		{{2 * description->square[0],	description->mixed[0],		description->mixed[1],		description->absolute[0]}},
		{{description->mixed[0],		2 * description->square[1],	description->mixed[2], 		description->absolute[1]}},
		{{description->mixed[1],		description->mixed[2],		2 * description->square[2],	description->absolute[2]}},
		{{description->absolute[0],		description->absolute[1],	description->absolute[2],	2 * description->constant}}
	}};
/*
	vector3_t min, max;

	for (std::size_t i = 0; i < 3; ++i)
	{
		const float d = std::sqrt(m[W][i] * m[W][i] - m[W][W] * m[i][i]) + std::numeric_limits<float>::min();
		min[i] = m[W][W] / (m[W][i] + d);
		max[i] = m[W][W] / (m[W][i] - d);
	}

	const box_t box = transform(description->transformation, box_t(min, max));
*/
	const bool invertible = m[0][0] && m[1][1] && m[2][2] && m[3][3];
	const box_t box = invertible ? foo(description->transformation, m) : bar(description->transformation, m);
// ---

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>(description->transformation, std::move(m)),
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
