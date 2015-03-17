/*
 * description.hpp
 *
 *  Created on: 11.02.2015
 *      Author: mgresens
 */

#pragma once

#include <math/vector.hpp>
#include <scene/description.hpp>
#include <scene/camera/description.hpp>
#include <scene/light/description.hpp>
#include <scene/object/description.hpp>
#include <scene/object/surface/description.hpp>
#include <scene/object/texture/noise/description.hpp>
#include <boost/unordered_map.hpp>
#include <boost/variant.hpp>
#include <string>

namespace rt {
namespace parsing {
namespace variable {

typedef boost::variant
<
	int,
	float,
	std::string,
	vector2_t,
	vector3_t,
	vector4_t,
	scene::description_t,
	scene::camera::description_t,
	scene::light::description_t,
	scene::object::description_t,
	scene::object::surface::description_t,
	scene::object::texture::description_t,
	scene::object::texture::noise::description_t
> description_t;

typedef boost::unordered_map<std::string, description_t> descriptions_t;

}
}
}
