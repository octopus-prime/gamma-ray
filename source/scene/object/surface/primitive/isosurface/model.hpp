/*
 * model.hpp
 *
 *  Created on: 12.05.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/isosurface/model_fwd.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace isosurface {

typedef std::pair<box_t, boost::none_t> value_t;
typedef geo::index::rtree<value_t, geo::index::rstar<8>> rtree_t;

class model
{
public:
	model(function_t&& function, rtree_t&& rtree)
	:
		_function(std::forward<function_t>(function)),
		_rtree(std::forward<rtree_t>(rtree))
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const auto function1 = [this, &ray](const float distance) -> float
		{
			const vector3_t point = ray.origin() + ray.direction() * distance;
			return _function(point);
		};

		const auto function = [&ray, &function1](const float distance) -> boost::math::tuple<float, float>
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

			boost::uintmax_t max = 100;
			const float t = boost::math::tools::newton_raphson_iterate
			(
				function,
				l - 0.1f + std::numeric_limits<float>::epsilon(),
				l - 0.1f,
				l + 0.1f,
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
	}

	bool
	inside(const vector3_t& point) const
	{
		return _function(point) >= 0.f;
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		constexpr float D = 1e-2;
		constexpr vector3_t X {{D, 0, 0}};
		constexpr vector3_t Y {{0, D, 0}};
		constexpr vector3_t Z {{0, 0, D}};

		return vector3_t
		{{
			_function(point + X) - _function(point - X),
			_function(point + Y) - _function(point - Y),
			_function(point + Z) - _function(point - Z)
		}};
	}

private:
	function_t _function;
	rtree_t _rtree;
};

}
}
}
}
}
}
