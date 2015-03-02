/*
 * ray.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <math/homogeneous.hpp>

namespace rt {
namespace rendering {

class ray_t
{
	static constexpr float epsilon = 1e-3;

public:
//	ray_t(const vector3_t& origin, const vector3_t& direction, const float min = 0.0f, const float max = std::numeric_limits<float>::max()) noexcept
	ray_t(const vector3_t& origin, const vector3_t& direction, const float min = 0.0f, const float max = 1e5f) noexcept
	:
		_origin(origin),
		_direction(direction),
		_min(min + epsilon),
		_max(max - epsilon)
	{
	}

	const vector3_t& origin() const noexcept
	{
		return _origin;
	}

	const vector3_t& direction() const noexcept
	{
		return _direction;
	}

	float min() const noexcept
	{
		return _min;
	}

	float max() const noexcept
	{
		return _max;
	}

	vector3_t operator()(const float distance) const noexcept
	{
		return _origin + _direction * distance;
	}

	bool operator[](const float distance) const noexcept
	{
		return _min < distance && distance < _max;
	}

	ray_t operator()(const matrix44_t& transformation) const noexcept
	{
		return
		{
			point::operator*(transformation, _origin),
			direction::operator*(transformation, _direction),
			_min,
			_max,
		};
	}

private:
	vector3_t _origin;
	vector3_t _direction;
	float _min;
	float _max;
};

}
}
