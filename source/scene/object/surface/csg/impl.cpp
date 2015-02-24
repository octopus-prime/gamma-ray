/*
 * impl.cpp
 *
 *  Created on: 08.01.2014
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/impl.hpp>
#include <scene/object/surface/csg/difference/model.hpp>
#include <scene/object/surface/csg/intersection/model.hpp>
#include <scene/object/surface/csg/union/model.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

template <typename Model>
impl<Model>::impl(const base_t& surface1, const base_t& surface2)
:
	base(),
	_model(surface1, surface2)
{
}

template <typename Model>
hits_t::iterator
impl<Model>::hit(const ray_t& ray, const hits_t::iterator hits) const
{
	return _model.hit(ray, hits);
}

template<typename Model>
bool
impl<Model>::inside(const vector3_t& point) const
{
	return _model.inside(point);
}

template class impl<difference::model>;
template class impl<intersection::model>;
template class impl<union_::model>;

}
}
}
}
}
