/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/sphere/make.hpp>
#include <scene/object/surface/primitive/sphere/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<sphere::model>;

namespace sphere {

surface::instance_t
make(const description_t& description)
{
	matrix44_t transformation = identity<4>();
	if (description->radius)
		transformation *= rt::scale({{*description->radius, *description->radius, *description->radius}});
	if (description->origin)
		transformation *= rt::translate(*description->origin);
	return primitive::make<model>(transformation * description->transformation);
}

}
}
}
}
}
}
