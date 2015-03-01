/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/texture/make.hpp>
#include <scene/object/texture/noise/make.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {

instance_t
make(const description_t& description)
{
	const instance_t instance
	(
		[description](const vector3_t& point) -> vector3_t
		{
			return description->pigment;
		},
		[description](const vector3_t& point) -> vector3_t
		{
			return description->bump;
		},
		noise::make(description->ambient),
		noise::make(description->diffuse),
		noise::make(description->specular),
		noise::make(description->shininess),
		noise::make(description->reflection),
		noise::make(description->transparency),
		noise::make(description->refraction)
	);
	return std::move(instance);
}

}
}
}
}
