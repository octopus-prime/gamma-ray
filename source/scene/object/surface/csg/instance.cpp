/*
 * instance.cpp
 *
 *  Created on: 08.01.2014
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/instance.hpp>
#include <scene/object/surface/csg/difference/model.hpp>
#include <scene/object/surface/csg/intersection/model.hpp>
#include <scene/object/surface/csg/union/model.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

template <typename Model>
instance<Model>::instance(const surface::instance_t& surface1, const surface::instance_t& surface2)
:
	surface::instance(),
	_model(surface1, surface2)
{
}

template <typename Model>
rendering::hits_t::iterator
instance<Model>::hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
{
	return _model.hit(ray, hits);
}

template<typename Model>
bool
instance<Model>::inside(const vector3_t& point) const
{
	return _model.inside(point);
}

template class instance<difference::model>;
template class instance<intersection::model>;
template class instance<union_::model>;

}
}
}
}
}
