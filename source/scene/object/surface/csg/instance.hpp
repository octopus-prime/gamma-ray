/*
 * impl.hpp
 *
 *  Created on: 08.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/instance.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

template <typename Model>
class instance
:
	public virtual surface::instance
{
public:
	instance(const surface::instance_t& surface1, const surface::instance_t& surface2);
	virtual rendering::hits_t::iterator hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const override;
	virtual bool inside(const vector3_t& point) const override;

private:
	Model _model;
};

template<typename Model>
surface::instance_t
make(const surface::instance_t& surface1, const surface::instance_t& surface2)
{
	return boost::make_shared<instance<Model>>(surface1, surface2);
}

}
}
}
}
}
