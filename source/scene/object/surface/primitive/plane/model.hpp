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
	model(vector3_t&& normal)
	:
		_normal(std::forward<vector3_t>(normal))
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const polynomial1_t polynomial
		{{
			_normal * ray.origin(),
			_normal * ray.direction()
		}};

		const rendering::distance_iterator begin(hits);
		const rendering::distance_iterator end = solve(polynomial, begin);

		std::for_each
		(
			hits, hits + std::distance(begin, end),
			[this, &ray](rendering::hit_t& hit)
			{
				hit.normal = _normal;
			}
		);

		return hits + std::distance(begin, end);
	}

	bool
	inside(const vector3_t& point) const
	{
		return false; // TODO
	}

private:
	vector3_t _normal;
};

}
}
}
}
}
}
