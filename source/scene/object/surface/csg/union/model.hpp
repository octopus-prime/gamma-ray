/*
 * model.hpp
 *
 *  Created on: 10.01.2014
 *      Author: mike_gresens
 */

#pragma once

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {
namespace union_ {

class model
{
public:
	model(const base_t& surface1, const base_t& surface2)
	:
		_surface1(surface1),
		_surface2(surface2)
	{
	}

	hits_t::iterator
	hit(const ray_t& ray, const hits_t::iterator hits) const
	{
		const hits_t::iterator end1 = _surface1->hit(ray, hits);
		const hits_t::iterator end2 = _surface2->hit(ray, end1);
		const hits_t::iterator hits1 = std::copy_if
		(
			hits, end1,
			hits,
			[this](const hit_t& hit)
			{
				return !_surface2->inside(hit.point);
			}
		);
		const hits_t::iterator hits2 = std::copy_if
		(
			end1, end2,
			hits1,
			[this](const hit_t& hit)
			{
				return !_surface1->inside(hit.point);
			}
		);
		return hits2;
	}

	bool
	inside(const vector3_t& point) const
	{
		return _surface1->inside(point) || _surface2->inside(point);
	}

private:
	base_t _surface1;
	base_t _surface2;
};

}
}
}
}
}
}
