/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <boost/math/quaternion.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace fractal {

class model
{
	typedef boost::math::quaternion<float> Q;

	class julia_t
	{
	public:
		Q operator()(const Q& z) const
		{
			return z * z + _c;
		};

		float operator()(const vector3_t& point) const
		{
			Q q(point[X], point[Y], point[Z], (_d - _s * point) / _t);
			Q r = _c;

			for (std::size_t i = 0; i < _n; ++i)
			{
				if (boost::math::abs(q) > _e)
					break;
				r = 2.0f * q * r;
				q = (*this)(q);
			}

			const float R = boost::math::l1(r);
			const float Q = boost::math::l1(q);

			return 0.5f * std::log(Q) * Q / R;
		}

	private:
		Q _c = Q(-0.08, 0.0, -0.8, -0.03);
		std::size_t _n = 20;
		rt::vector3_t _s = {{0,0,0}};
		float _t = 1;
		float _d = 0;
		float _e = 4;
	};

public:
	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		float t = ray.min();

		for (int i = 0; i < 100; ++i)
		{
			const vector3_t p = ray(t);
			const float e = _julia(p);

//			if (!ray[t])
//				return hits;

			if (e < 1e-4)
			{
				hits->distance = t;
				hits->normal = p;	// TODO
				return hits + 1;
			}

			t += e;
		}

		return hits;
	}

	bool
	inside(const vector3_t& point) const
	{
		return false; // todo
	}

protected:
private:
	julia_t _julia;
};

}
}
}
}
}
}
