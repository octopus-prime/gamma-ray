/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <geo/segment.hpp>
#include <geo/box.hpp>
#include <boost/math/quaternion.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace fractal {

typedef boost::math::quaternion<float> quaternion_t;
typedef boost::function<float (vector3_t)> estimation_t;

typedef std::pair<box_t, bool> value_t;
typedef geo::index::rtree<value_t, geo::index::rstar<16>> rtree_t; // TODO: parse parameter

class model
{
public:
	model(estimation_t&& estimate, const float precision, rtree_t&& rtree)
	:
		_estimate(std::forward<estimation_t>(estimate)),
		_precision(precision),
		_rtree(std::forward<rtree_t>(rtree))
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const segment_t segment = ray;
		value_t value(box_t(), false);
		_rtree.query(geo::index::intersects(segment) && geo::index::nearest(segment.first, 1), &value);
		if (!value.second)
			return hits;

		const float start = length(0.5f * (value.first.min_corner() + value.first.max_corner()));

		for (float distance = start; ray[distance]; )
		{
			const vector3_t point = ray(distance);
			const float estimation = _estimate(point);

			if (estimation < _precision)
			{
				hits->distance = distance;
				hits->normal = normal(point);
				return hits + 1;
			}

			distance += estimation;
		}

		return hits;
	}

	bool
	inside(const vector3_t& point) const
	{
		return false; // todo
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		constexpr float D = 0.25e-1;
		constexpr vector3_t X {{D, 0, 0}};
		constexpr vector3_t Y {{0, D, 0}};
		constexpr vector3_t Z {{0, 0, D}};

		return vector3_t
		{{
			_estimate(point + X) - _estimate(point - X),
			_estimate(point + Y) - _estimate(point - Y),
			_estimate(point + Z) - _estimate(point - Z)
		}};
	}

private:
	estimation_t _estimate;
	float _precision;
	rtree_t _rtree;
};

}
}
}
}
}
}
