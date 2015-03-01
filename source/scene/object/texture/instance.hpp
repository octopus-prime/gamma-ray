/*
 * instance.hpp
 *
 *  Created on: 28.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <boost/function.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {

template <typename T>
using procedural_t = boost::function<T (vector3_t)>;

class instance_t
{
public:
	instance_t
	(
		const procedural_t<vector3_t>& pigment,
		const procedural_t<vector3_t>& bump,
		const procedural_t<float>& ambient,
		const procedural_t<float>& diffuse,
		const procedural_t<float>& specular,
		const procedural_t<float>& shininess,
		const procedural_t<float>& reflection,
		const procedural_t<float>& transparency,
		const procedural_t<float>& refraction
	)
	:
		_pigment(pigment),
		_bump(bump),
		_ambient(ambient),
		_diffuse(diffuse),
		_specular(specular),
		_shininess(shininess),
		_reflection(reflection),
		_transparency(transparency),
		_refraction(refraction)
	{
	}

	vector3_t
	pigment(const vector3_t& point) const noexcept
	{
		return _pigment(point);
	}

	vector3_t
	bump(const vector3_t& point) const noexcept
	{
		return _bump(point);
	}

	float
	ambient(const vector3_t& point) const noexcept
	{
		return _ambient(point);
	}

	float
	diffuse(const vector3_t& point) const noexcept
	{
		return _diffuse(point);
	}

	float
	specular(const vector3_t& point) const noexcept
	{
		return _specular(point);
	}

	float
	shininess(const vector3_t& point) const noexcept
	{
		return _shininess(point);
	}

	float
	reflection(const vector3_t& point) const noexcept
	{
		return _reflection(point);
	}

	float
	transparency(const vector3_t& point) const noexcept
	{
		return _transparency(point);
	}

	float
	refraction(const vector3_t& point) const noexcept
	{
		return _refraction(point);
	}

private:
	procedural_t<vector3_t> _pigment;
	procedural_t<vector3_t> _bump;
	procedural_t<float> _ambient;
	procedural_t<float> _diffuse;
	procedural_t<float> _specular;
	procedural_t<float> _shininess;
	procedural_t<float> _reflection;
	procedural_t<float> _transparency;
	procedural_t<float> _refraction;
};

}
}
}
}
