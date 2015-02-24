/*
 * model.hpp
 *
 *  Created on: 22.01.2012
 *      Author: mike
 */

#pragma once

#include <math/matrix.hpp>
#include <math/homogeneous.hpp>
#include <math/polynomial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace quadric {

class model
{
public:
	constexpr
	model(matrix44_t&& matrix)
	:
		_matrix(std::forward<matrix44_t>(matrix))
	{
	}

	hits_t::iterator
	hit(const ray_t& ray, const hits_t::iterator hits) const
	{
		const vector4_t om = point::operator*(ray.origin, _matrix);
		const vector4_t dm = direction::operator*(ray.direction, _matrix);
		const polynomial2_t polynomial
		{{
			point::operator*(om, ray.origin),
			2.0f * point::operator*(dm, ray.origin),
			direction::operator*(dm, ray.direction)
		}};
		const distance_iterator begin(hits);
		const distance_iterator end = solve(polynomial, begin);
		std::for_each
		(
			hits, hits + std::distance(begin, end),
			[this, &ray](hit_t& hit)
			{
				hit.normal = normal(ray(hit.distance));
			}
		);
		return hits + std::distance(begin, end);
	}

	bool
	inside(const vector3_t& point) const
	{
		return point::operator*(point::operator*(point, _matrix), point) <= 0.0f;
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		return normalize(point::operator*(_matrix, point));
	}

private:
	matrix44_t _matrix;
};

}
}
}
}
}
}
