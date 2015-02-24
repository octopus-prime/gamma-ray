/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace cube {

class model
{
public:
	hits_t::iterator
	hit(const ray_t& ray, const hits_t::iterator hits) const
	{
		hits[0].distance = ray.min;
		hits[1].distance = ray.max;
		return hits + 2 * (test<X>(ray, hits) && test<Y>(ray, hits) && test<Z>(ray, hits));
	}

	bool
	inside(const vector3_t& point) const
	{
		return test<X>(point) && test<Y>(point) && test<Z>(point);
	}

protected:
	template <axis_t N>
	static bool
	test(const vector3_t& point)
	{
		return point[N] >= POINT[0][N] && point[N] <= POINT[1][N];
	}

	template <axis_t N>
	static bool
	test(const ray_t& ray, const hits_t::iterator hits)
	{
		const bool sign = std::signbit(ray.direction[N]);
		const float min = (POINT[ sign][N] - ray.origin[N]) / ray.direction[N];
		const float max = (POINT[!sign][N] - ray.origin[N]) / ray.direction[N];

		if (hits[0].distance > max || min > hits[1].distance)
			return false;

		if (min > hits[0].distance)
		{
			hits[0].distance = min;
			hits[0].normal = NORMAL[sign][N];
		}
		if (max < hits[1].distance)
		{
			hits[1].distance = max;
			hits[1].normal = NORMAL[!sign][N];
		}

		return true;
	}

private:
	static constexpr matrix23_t POINT
	{{
		{{-1, -1, -1}},
		{{+1, +1, +1}}
	}};

	static constexpr basic_container_t<matrix33_t, 2> NORMAL
	{{
		{{
			{{-1,0,0}}, {{0,-1,0}}, {{0,0,-1}}
		}},
		{{
			{{+1,0,0}}, {{0,+1,0}}, {{0,0,+1}}
		}}
	}};
};

}
}
}
}
}
}
