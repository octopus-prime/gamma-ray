/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/fractal/make.hpp>
#include <scene/object/surface/primitive/fractal/model.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<fractal::model>;

namespace fractal {

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface fractal";

	const quaternion_t constant
	(
		description->constant[0],
		description->constant[1],
		description->constant[2],
		description->constant[3]
	);

	const std::size_t iterations = description->iterations;

	const vector3_t slice
	{{
		description->slice[X],
		description->slice[Y],
		description->slice[Z]
	}};

	const float length = description->slice[W];
	const float distance = description->distance;

	// The distance-estimation function
	estimation_t estimate = [constant, iterations, slice, length, distance](const vector3_t& point) -> float
	{
		constexpr float exit = 4;

		const auto function = [constant](const quaternion_t& value)
		{
			return value * value + constant;
		};

		quaternion_t value(point[X], point[Y], point[Z], (distance - slice * point) / length);
		quaternion_t derivation = constant;

		for (std::size_t iteration = 0; iteration < iterations; ++iteration)
		{
			if (boost::math::abs(value) > exit)
				break;

			derivation = 2.0f * value * derivation;
			value = function(value);
		}

		const float derivation_norm = boost::math::l1(derivation);
		const float value_norm = boost::math::l1(value);

		return 0.5f * std::log(value_norm) * value_norm / derivation_norm;
	};

	// Build rtree
	constexpr vector3_t MIN {{-2, -2, -2}};
	constexpr vector3_t MAX {{+2, +2, +2}};
	constexpr std::size_t N = 100;
	const vector3_t DELTA = (MAX - MIN) / N;
	const float RADIUS = 0.5f * rt::length(DELTA);

	rtree_t rtree;

	for (std::size_t x = 0; x < N; ++x)
		for (std::size_t y = 0; y < N; ++y)
			for (std::size_t z = 0; z < N; ++z)
			{
				const vector3_t delta = DELTA & vector3_t {{float(x), float(y), float(z)}};
				const vector3_t min = MIN + delta;
				const vector3_t max = min + DELTA;
				const vector3_t point = 0.5f * (min + max);
				const float e = estimate(point);
				if (e >= 0 && e <= RADIUS)
					rtree.insert(value_t(box_t(min, max), true));
			}

	// Build box
	const box_t box = transform
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
			std::move(estimate),
			description->precision,
			std::move(rtree)
		),
		box,
		1
	);
}

}
}
}
}
}
}
