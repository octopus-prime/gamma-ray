/*
 * description.hpp
 *
 *  Created on: 20.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/description.hpp>
#include <scene/object/surface/description.hpp>
#include <scene/object/texture/description.hpp>
#include <scene/object/texture/noise/description.hpp>

namespace rt {
namespace parsing {
namespace transformation {

typedef boost::variant
<
	scene::object::description_t,
	scene::object::surface::description_t,
	scene::object::texture::description_t,
	scene::object::texture::noise::description_t
> description_t;

}
}
}
