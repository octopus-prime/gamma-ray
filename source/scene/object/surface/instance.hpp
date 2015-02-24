/*
 * base.hpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <rendering/ray.hpp>
#include <rendering/hit.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {

class instance
{
protected:
	virtual ~instance() noexcept = default;

public:
	virtual rendering::hits_t::iterator hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const = 0;
	virtual bool inside(const vector3_t& point) const = 0;
};

typedef boost::shared_ptr<instance> instance_t;

}
}
}
}
