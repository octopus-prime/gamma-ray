/*
 * model.hpp
 *
 *  Created on: 12.05.2014
 *      Author: mike_gresens
 */

#pragma once

#include <boost/math/tools/roots.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace blob {

typedef std::pair<vector3_t, float> carrier_t;
typedef std::vector<carrier_t> carriers_t;

typedef std::pair<box_t, boost::none_t> value_t;
typedef geo::index::rtree<value_t, geo::index::rstar<8>> rtree_t;

class model
{
public:
	model(carriers_t&& carriers, const float threshold, rtree_t&& rtree)
	:
		_carriers(std::forward<carriers_t>(carriers)),
		_threshold(threshold),
		_rtree(rtree)
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const auto function1 = [this, &ray](const float distance) -> float
		{
			const vector3_t point = ray.origin() + ray.direction() * distance;
			return potential(point) - _threshold;
		};

		const auto function = [this, &ray, &function1](const float distance) -> boost::math::tuple<float, float>
		{
			constexpr float h = 1e-2;
			return boost::math::make_tuple
			(
				function1(distance),
				(function1(distance + h) - function1(distance - h)) / (2 * h)
			);
		};

		rendering::hits_t::iterator end = hits;
		const auto test = [this, &ray, &end, &function, &function1](const value_t& value)
		{
			const box_t& box = value.first;
			const vector3_t point = 0.5f * (box.min_corner() + box.max_corner());
			const float l = length(point - ray.origin());

			if (!ray[l])
				return false;

//			end = triangle.hit(ray, end);
			boost::uintmax_t max = 100;
			const float t = boost::math::tools::newton_raphson_iterate
			(
				function,
				l - 0.1 + std::numeric_limits<float>::epsilon(),
				l - 0.1,
				l + 0.1,
				std::numeric_limits<float>::digits,
				max
			);

			const float f = std::abs(function1(t));

			if (f < 1e-4)
			{
				end->distance = t;
				end->normal = normal(ray(t));
				++end;
			}

			return false;
		};

		const segment_t segment = ray;
		_rtree.query(geo::index::intersects(segment) && geo::index::satisfies(test) /*&& geo::index::nearest(segment.first, 1)*/, (value_t*) nullptr);

		return end;
//		if (end == hits)
//			return hits;
/*
		boost::uintmax_t max = 200;
		const float t = boost::math::tools::newton_raphson_iterate
		(
			function,
			ray.min() + std::numeric_limits<float>::epsilon(),
			ray.min(),
			ray.max(),
			std::numeric_limits<float>::digits,
			max
		);

		const float f = std::abs(function1(t));

		if (f > 1e-6)
			return hits;

		hits->distance = t;
		hits->normal = normal(ray(t));
*/
//		return hits + 1;
	}

	bool
	inside(const vector3_t& point) const
	{
		return potential(point) >= _threshold;
	}

protected:
	float
	potential(const vector3_t& point) const
	{
		return boost::accumulate
		(
			_carriers,
			0.f,
			[&point](const float potential, const carrier_t& carrier)
			{
				const vector3_t vector = point - carrier.first;
				const float radius2 = vector * vector;
				return potential + carrier.second / radius2;
			}
		);
	}

	vector3_t
	normal(const vector3_t& point) const
	{
		return boost::accumulate
		(
			_carriers,
			vector3_t {{0, 0, 0}},
			[&point](const vector3_t& normal, const carrier_t& carrier)
			{
				const vector3_t vector = point - carrier.first;
				const float radius2 = vector * vector;
				return normal + (carrier.second / (radius2 * radius2)) * vector;
			}
		);
	}

private:
	carriers_t _carriers;
	float _threshold;
	rtree_t _rtree;
};

}
}
}
}
}
}
