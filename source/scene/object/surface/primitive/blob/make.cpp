/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/blob/make.hpp>
#include <scene/object/surface/primitive/blob/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<blob::model>;

namespace blob {
/*
template <axis_t N>
static std::pair<float,float>
calculate_bound(const std::vector<std::pair<vector3_t, float>>& carriers, const float threshold)
{
	polynomialN_t product { 1.0 };

	for (const auto& carrier : carriers)
	{
		const polynomial2_t p {{carrier.first[N] * carrier.first[N], -2.0 * carrier.first[N], 1.0 }};
		product = product * p;
	}

	polynomialN_t sum = product * threshold;

	for (const auto& carrier : carriers)
	{
		const polynomial2_t p {{carrier.first[N] * carrier.first[N], -2.0 * carrier.first[N], 1.0 }};
		sum -= (product / p) * carrier.second;
	}

	std::vector<double> roots(2 * carriers.size());
	const std::vector<double>::iterator end = solve(sum, roots.begin());
	const auto minmax = std::minmax_element(roots.begin(), end);

	return std::make_pair(*minmax.first, *minmax.second);
}

static bounding::box_t
calculate_box(const std::vector<std::pair<vector3_t, float>>& carriers, const float threshold)
{
	const std::pair<float,float> boundX = calculate_bound<X>(carriers, threshold);
	const std::pair<float,float> boundY = calculate_bound<Y>(carriers, threshold);
	const std::pair<float,float> boundZ = calculate_bound<Z>(carriers, threshold);
	return bounding::box_t
	(
		{{boundX.first,  boundY.first,  boundZ.first}},
		{{boundX.second, boundY.second, boundZ.second}}
	);
}
*/
boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface blob";

	box_t box({{-3,-3,-3}}, {{+3,+3,+3}});	// todo

	const auto potential = [&description](const vector3_t& point) -> float
	{
		return boost::accumulate
		(
			description->carriers,
			0.f,
			[&point](const float potential, const carrier_t& carrier)
			{
				const vector3_t vector = point - carrier.first;
				const float radius2 = vector * vector;
				return potential + carrier.second / radius2;
			}
		);
	};

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
				count += potential({{min[X], min[Y], min[Z]}}) >= description->threshold;
				count += potential({{min[X], min[Y], max[Z]}}) >= description->threshold;
				count += potential({{min[X], max[Y], min[Z]}}) >= description->threshold;
				count += potential({{min[X], max[Y], max[Z]}}) >= description->threshold;
				count += potential({{max[X], min[Y], min[Z]}}) >= description->threshold;
				count += potential({{max[X], min[Y], max[Z]}}) >= description->threshold;
				count += potential({{max[X], max[Y], min[Z]}}) >= description->threshold;
				count += potential({{max[X], max[Y], max[Z]}}) >= description->threshold;

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
			std::move(description->carriers),
			description->threshold,
			std::move(rtree)
		),
		box,
		2 * description->carriers.size()
	);
}

}
}
}
}
}
}
