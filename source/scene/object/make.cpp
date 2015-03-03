/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/texture/make.hpp>

namespace rt {
namespace scene {
namespace object {

instance_t
make(const description_t& description)
{
	const instance_t instance
	(
		surface::make(description->surface).get<0>(),
		texture::make(description->texture)
	);
	return std::move(instance);
}

}
}
}
