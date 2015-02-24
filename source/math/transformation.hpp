/*
 * transformation.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <math/matrix.hpp>

namespace rt {

inline matrix44_t
translate(const vector3_t& translation)
{
	return matrix44_t
	{{
		{{1.0,	0.0,	0.0,	-translation[X]}},
		{{0.0,	1.0,	0.0,	-translation[Y]}},
		{{0.0,	0.0,	1.0,	-translation[Z]}},
		{{0.0,	0.0,	0.0,	1.0}}
	}};
}

inline matrix44_t
rotate(const vector3_t& rotation)
{
	constexpr float f = -M_PI / 180.0;

	const auto rotateX = [&rotation]() -> matrix44_t
	{
		const float angle = f * rotation[X];
		const float s = std::sin(angle);
		const float c = std::cos(angle);
		return
		{{
			{{1.0,	0.0,	0.0,	0.0}},
			{{0.0,	+c,		-s,		0.0}},
			{{0.0,	+s,		+c,		0.0}},
			{{0.0,	0.0,	0.0,	1.0}}
		}};
	};

	const auto rotateY = [&rotation]() -> matrix44_t
	{
		const float angle = f * rotation[Y];
		const float s = std::sin(angle);
		const float c = std::cos(angle);
		return
		{{
			{{+c,	0.0,	+s,		0.0}},
			{{0.0,	1.0,	0.0,	0.0}},
			{{-s,	0.0,	+c,		0.0}},
			{{0.0,	0.0,	0.0,	1.0}}
		}};
	};

	const auto rotateZ = [&rotation]() -> matrix44_t
	{
		const float angle = f * rotation[Z];
		const float s = std::sin(angle);
		const float c = std::cos(angle);
		return
		{{
			{{+c,	-s,		0.0,	0.0}},
			{{+s,	+c,		0.0,	0.0}},
			{{0.0,	0.0,	1.0,	0.0}},
			{{0.0,	0.0,	0.0,	1.0}}
		}};
	};

	return rotateX() * rotateY() * rotateZ();
}

inline matrix44_t
scale(const vector3_t& scaling)
{
	return matrix44_t
	{{
		{{1.f / scaling[X],	0.0,				0.0,				0.0}},
		{{0.0,				1.f / scaling[Y],	0.0,				0.0}},
		{{0.0,				0.0,				1.f / scaling[Z],	0.0}},
		{{0.0,				0.0,				0.0,				1.f}}
	}};
}

}
