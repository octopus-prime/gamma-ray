/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/polynomial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace sphere {

class model
{
public:
	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const polynomial2_t polynomial
		{{
			ray.origin() * ray.origin() - 1.0f,
			ray.origin() * ray.direction() * 2.0f,
			ray.direction() * ray.direction()
		}};

		const rendering::distance_iterator begin(hits);
		const rendering::distance_iterator end = solve(polynomial, begin);

		std::for_each
		(
			hits, hits + std::distance(begin, end),
			[this, &ray](rendering::hit_t& hit)
			{
				hit.normal = normal(ray(hit.distance));
			}
		);

		return hits + std::distance(begin, end);
	}

	bool
	inside(const vector3_t& point) const
	{
		return length(point) <= 1.0f;
	}

protected:
	constexpr vector3_t
	normal(const vector3_t& point) const
	{
		return point;
	}
};

}
}
}
}
}
}
