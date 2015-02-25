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
namespace torus {

class model
{
public:
	model(const float major, const float minor)
	:
		_major(major),
		_minor(minor)
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const float a = ray.direction() * ray.direction();
		const float b = ray.origin() * ray.direction() * 2.0f;
		const float c = ray.origin() * ray.origin() - _minor * _minor - _major * _major;
		const float R = 4.0f * _major * _major;

		const polynomial4_t polynomial
		{{
			c * c + R * (ray.origin()[Z] * ray.origin()[Z] - _minor * _minor),
			2.0f * (b * c + R * ray.origin()[Z] * ray.direction()[Z]),
			b * b + 2.0f * a * c + R * ray.direction()[Z] * ray.direction()[Z],
			2.0f * a * b,
			a * a
		}};

		const rendering::distance_iterator roots_begin(hits);
		const rendering::distance_iterator roots_end = solve(polynomial, roots_begin);
		std::for_each
		(
			hits, hits + std::distance(roots_begin, roots_end),
			[this, &ray](rendering::hit_t& hit)
			{
				hit.normal = normal(ray(hit.distance));
			}
		);
		return hits + std::distance(roots_begin, roots_end);
	}

	bool
	inside(const vector3_t& point) const
	{
		const float delta = std::hypot(point[X], point[Y]) - _major;
		return point[Z] * point[Z] + delta * delta <= _minor * _minor;
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		const float length = 4.0f * (point * point - _minor * _minor - _major * _major);
		const vector3_t vector{{length, length, length + 8.0f * _major * _major}};
		return /*normalize(*/point & vector;//);
	}

private:
	float _major;
	float _minor;
};

}
}
}
}
}
}
