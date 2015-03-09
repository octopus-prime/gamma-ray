/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/sor/make.hpp>
#include <scene/object/surface/primitive/sor/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<sor::model>;

namespace sor {

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface sor";

	const auto& points = description->points;
	spline_t spline = make_spline(points.begin(), points.end());
	derivations_t derivations;
	rtree_t rtree;

	for (std::size_t i = 0; i < spline.size(); ++i)
	{
		const spline_segment_t& segment = spline[i];
		const polynomial5_t derivation = differentiate(segment.get<2>() * segment.get<2>());

		const float delta = segment.get<1>() - segment.get<0>();
		float max = std::max
		(
			(float)evaluate(segment.get<2>(), 0.0),
			(float)evaluate(segment.get<2>(), delta)
		);

		boost::array<double, 5> roots;
		const auto end = solve(derivation, roots.begin());
		for (auto root = roots.begin(); root != end; ++root)
			if (*root >= 0.0 && *root <= delta)
				max = std::max
				({
					max,
					(float)evaluate(segment.get<2>(), *root)
				});

		const box_t box
		(
			{{-max, segment.get<0>(), -max}},
			{{+max, segment.get<1>(), +max}}
		);

		derivations.emplace_back(derivation);
		rtree.insert(value_t(box, i));
	}

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

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>
		(
			description->transformation,
			std::move(spline),
			std::move(derivations),
			std::move(rtree),
			points.front()[X],
			points.back()[X],
			points.front()[Y],
			points.back()[Y]
		),
		box,
		3 * spline.size() + 2
	);
}

}
}
}
}
}
}
