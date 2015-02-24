/*
 * impl.hpp
 *
 *  Created on: 08.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/base.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

template <typename Model>
class impl
:
	public virtual base
{
public:
	impl(const base_t& surface1, const base_t& surface2);
	virtual hits_t::iterator hit(const ray_t& ray, const hits_t::iterator hits) const override;
	virtual bool inside(const vector3_t& point) const override;

private:
	Model _model;
};

template<typename Model>
base_t make(const base_t& surface1, const base_t& surface2)
{
	return boost::make_shared<impl<Model>>(surface1, surface2);
}

}
}
}
}
}
