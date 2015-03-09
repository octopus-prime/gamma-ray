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
namespace plane {

class model
{
public:
	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		static constexpr vector3_t normal {{0,1,0}};

		const polynomial1_t polynomial
		{{
			normal * ray.origin(),
			normal * ray.direction()
		}};

		const rendering::distance_iterator begin(hits);
		const rendering::distance_iterator end = solve(polynomial, begin);

		std::for_each
		(
			hits, hits + std::distance(begin, end),
			[this, &ray](rendering::hit_t& hit)
			{
				hit.normal = normal;
			}
		);

		return hits + std::distance(begin, end);
	}

	bool
	inside(const vector3_t& point) const
	{
		return false; // TODO
	}

};

}
}
}
}
}
}
