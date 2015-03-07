/*
 * instance.cpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/instance.hpp>
#include <scene/object/surface/primitive/sphere/model.hpp>
#include <scene/object/surface/primitive/cube/model.hpp>
#include <scene/object/surface/primitive/quadric/model.hpp>
#include <scene/object/surface/primitive/torus/model.hpp>
#include <scene/object/surface/primitive/mesh/model.hpp>
#include <scene/object/surface/primitive/text/model.hpp>
#include <boost/iterator/filter_iterator.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

template <typename Model>
instance<Model>::instance(const matrix44_t& transformation, Model&& model)
:
	surface::instance(),
	_scene_object(transformation),
	_object_scene(transpose(transformation)),
	_model(std::forward<Model>(model))
{
}

template <typename Model>
rendering::hits_t::iterator
instance<Model>::hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
{
	const auto end = _model.hit(ray(_scene_object), hits);
	const auto check = [&ray](const rendering::hit_t& hit){return ray[hit.distance];};
	return std::transform
	(
		boost::make_filter_iterator(check, hits, end),
		boost::make_filter_iterator(check, end, end),
		hits,
		[this, &ray](const rendering::hit_t& hit)
		{
			return rendering::hit_t
			{
				hit.distance,
				ray(hit.distance),
//				normalize(direction::operator*(_object_scene, hit.normal)),
				direction::operator*(_object_scene, hit.normal),
				nullptr
			};
		}
	);
}

template <typename Model>
bool
instance<Model>::inside(const vector3_t& point) const
{
	return _model.inside(point::operator*(_scene_object, point));
}

constexpr matrix23_t cube::model::POINT;
constexpr basic_container_t<matrix33_t, 2> cube::model::NORMAL;

template class instance<sphere::model>;
template class instance<cube::model>;
template class instance<quadric::model>;
template class instance<torus::model>;
template class instance<mesh::model>;
template class instance<text::model>;

}
}
}
}
}
