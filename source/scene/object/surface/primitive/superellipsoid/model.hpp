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
//		return std::pow(std::pow(std::abs(point[X]), _e) + std::pow(std::abs(point[Z]), _e), _n/_e) + std::pow(std::abs(point[Y]), _n);
		return length(point); // for test e = n = 1
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

	float
	operator()(const float t) const
	{
		const vector3_t point = _ray(t);
		return _function(point) - 1.f;
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
		_e(2.f / e),
		_n(2.f / n),
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
			const float x0 = distance[0];// - std::numeric_limits<float>::epsilon();
			const float x1 = distance[1];// + std::numeric_limits<float>::epsilon();

//			if (min > x0 || x0 > max || min > x1 || x1 > max)
//				continue;

			const float f0 = function(x0);
			const float f1 = function(x1);

			if (std::signbit(f0) == std::signbit(f1))
				continue;

			try
			{
				boost::uintmax_t max = std::numeric_limits<boost::uintmax_t>::max();
				const std::pair<float, float> found = boost::math::tools::toms748_solve
				(
					function,
					x0,	x1,
					f0, f1,
					boost::math::tools::eps_tolerance<float>(std::numeric_limits<float>::digits),
					max
				);
				const float t = 0.5f * (found.first + found.second);
				if (ray[t])
				{
					end->distance = t;
					end->normal = normal(ray(t));
					++end;
				}
			}
			catch (...)
			{
				std::cout << '.';
			}
		}
		return end;
	}

	bool
	inside(const vector3_t& point) const
	{
		return _function(point) <= 1.0;
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		return point; // test for e = n = 1
/*
		const float p = std::pow(std::abs(point[Y]), _n);
		vector3_t result {{1 - p, p, 1 - p}};

		if (std::abs(point[X]) > std::abs(point[Z]))
		{
			const float q = std::pow(std::abs(point[Z] / point[X]), _e);
			result &= {{1, 1 + q, q}};
		}
		else
		{
			const float q = std::pow(std::abs(point[X] / point[Z]), _e);
			result &= {{q, 1 + q, 1}};
		}

		return result | point;
*/
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
