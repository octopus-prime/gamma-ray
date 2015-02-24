/*
 * instance.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace light {

class instance_t
{
public:
	instance_t(const vector3_t& origin, const vector3_t& color) noexcept
	:
		_origin(origin),
		_color(color)
	{
	}

	const vector3_t& origin() const noexcept
	{
		return _origin;
	}

	const vector3_t& color() const noexcept
	{
		return _color;
	}

private:
	vector3_t _origin;
	vector3_t _color;
};

typedef std::vector<instance_t> instances_t;

}
}
}
