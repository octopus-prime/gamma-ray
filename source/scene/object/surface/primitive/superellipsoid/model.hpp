/*
 * model.hpp
 *
 *  Created on: 12.05.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/plane/model.hpp>
#include <scene/object/surface/primitive/cube/model.hpp>
#include <boost/math/tools/roots.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace superellipsoid {

class point_function_t
{
public:
	constexpr
	point_function_t(const float e, const float n)
	:
		_e(e),
		_n(n)
	{
	}

	float
	operator()(const vector3_t& point) const
	{
		const vector3_t s = point & point;
		return std::pow(std::pow(s[X], _e) + std::pow(s[Z], _e), _n/_e) + std::pow(s[Y], _n) - 1.f;
	}

private:
	float _e;
	float _n;
};

class distance_function_t
{
public:
	constexpr
	distance_function_t(const point_function_t& function, const rendering::ray_t& ray)
	:
		_function(function),
		_ray(ray)
	{
	}

	boost::math::tuple<float, float>
	operator()(const float t) const
	{
		constexpr float h = 1e-4;
		return boost::math::make_tuple
		(
			function(t),
			(function(t + h) - function(t - h)) / (2 * h)
		);
	}

protected:
	float
	function(const float t) const
	{
		const vector3_t point = _ray(t);
		return _function(point);
	}

private:
	const point_function_t& _function;
	const rendering::ray_t& _ray;
};

class model
{
public:
	model(const float e, const float n)
	:
		_e(1.f / e),
		_n(1.f / n),
		_function(_e, _n),
		_cube(),
		_planes
		{{
			plane::model(normalize(vector3_t {{1, 1, 0}})),
			plane::model(normalize(vector3_t {{1,-1, 0}})),
			plane::model(normalize(vector3_t {{1, 0, 1}})),
			plane::model(normalize(vector3_t {{1, 0,-1}})),
			plane::model(normalize(vector3_t {{0, 1, 1}})),
			plane::model(normalize(vector3_t {{0, 1,-1}})),
			plane::model(normalize(vector3_t {{1, 0, 0}})),
			plane::model(normalize(vector3_t {{0, 1, 0}})),
			plane::model(normalize(vector3_t {{0, 0, 1}}))
		}}
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		rendering::hits_t::iterator end = _cube.hit(ray, hits);

		if (end == hits)
			return hits;

		const float min = std::min(hits[0].distance, hits[1].distance);
		const float max = std::max(hits[0].distance, hits[1].distance);

		for (const auto& plane : _planes)
			end = plane.hit(ray, end);

		const rendering::distance_iterator distance_begin(hits);
		const rendering::distance_iterator distance_end(end);

		std::sort(distance_begin, distance_end);

		const distance_function_t function(_function, ray);

		end = hits;
		for (auto distance = distance_begin; distance != distance_end - 1; ++distance)
		{
			const float x0 = distance[0];
			const float x1 = distance[1];

			if (min > x0 || x0 > max || min > x1 || x1 > max)
				continue;

			boost::uintmax_t max = 1000;
			const float t = boost::math::tools::newton_raphson_iterate
			(
				function,
				(x0 + x1) / 2,
				x0,
				x1,
				std::numeric_limits<float>::digits,
				max
			);

			const float f = std::abs(boost::math::get<0>(function(t)));

			if (f < 1e-4 && ray[t])
			{
				end->distance = t;
				end->normal = normal(ray(t));
//				++end;
				return end + 1;
			}
		}

		return end;
	}

	bool
	inside(const vector3_t& point) const
	{
		return _function(point) <= 0.0;
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		const vector3_t s = point & point;

		const float p = std::pow(s[Y], _n);
		vector3_t result {{1 - p, p, 1 - p}};

		if (s[X] > s[Z])
		{
			const float q = std::pow(s[Z] / s[X], _e);
			result &= {{1, 1 + q, q}};
		}
		else
		{
			const float q = std::pow(s[X] / s[Z], _e);
			result &= {{q, 1 + q, 1}};
		}

		return result | point;

	}

private:
	float _e;
	float _n;
	point_function_t _function;
	cube::model _cube;
	boost::array<plane::model, 9> _planes;
};

}
}
}
}
}
}
