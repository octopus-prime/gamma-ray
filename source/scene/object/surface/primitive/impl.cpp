/*
 * impl.cpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/impl.hpp>
#include <scene/object/surface/primitive/sphere/model.hpp>
#include <scene/object/surface/primitive/cube/model.hpp>
#include <scene/object/surface/primitive/quadric/model.hpp>
#include <boost/iterator/filter_iterator.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

template <typename Model>
impl<Model>::impl(const matrix44_t& transformation, Model&& model)
:
	base(),
	_scene_object(transformation),
	_object_scene(transpose(transformation)),
	_model(std::forward<Model>(model))
{
}

template <typename Model>
hits_t::iterator
impl<Model>::hit(const ray_t& ray, const hits_t::iterator hits) const
{
	const auto end = _model.hit(ray(_scene_object), hits);
	const auto check = [&ray](const hit_t& hit){return ray[hit.distance];};
	return std::transform
	(
		boost::make_filter_iterator(check, hits, end),
		boost::make_filter_iterator(check, end, end),
		hits,
		[this, &ray](const hit_t& hit)
		{
			return hit_t
			{
				hit.distance,
				ray(hit.distance),
				normalize(direction::operator*(_object_scene, hit.normal))
//				nullptr
			};
		}
	);
}

template <typename Model>
bool
impl<Model>::inside(const vector3_t& point) const
{
	/*
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	const bool i0 = _model.inside(point::operator*(_scene_object, point));
	std::cout << "i0 = " << std::boolalpha << i0 << std::endl;
	return i0;
	*/
	return _model.inside(point::operator*(_scene_object, point));
}

constexpr matrix23_t cube::model::POINT;
constexpr basic_container_t<matrix33_t, 2> cube::model::NORMAL;

template class impl<sphere::model>;
template class impl<cube::model>;
template class impl<quadric::model>;

}
}
}
}
}
