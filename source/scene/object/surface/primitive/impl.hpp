/*
 * impl.hpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/base.hpp>
#include <math/matrix.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

template <typename Model>
class impl
:
	public virtual base
{
public:
	impl(const matrix44_t& transformation, Model&& model);

	virtual hits_t::iterator hit(const ray_t& ray, const hits_t::iterator hits) const override;
	virtual bool inside(const vector3_t& point) const override;

private:
	matrix44_t _scene_object;
	matrix44_t _object_scene;
	Model _model;
};

template<typename Model, typename... Args>
base_t make(const matrix44_t& transformation, Args&&... args)
{
	Model model(std::forward<Args>(args)...);
	return boost::make_shared<impl<Model>>(transformation, std::move(model));
}

}
}
}
}
}
