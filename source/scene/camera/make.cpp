/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/camera/make.hpp>

namespace rt {
namespace scene {
namespace camera {

instance_t
make(const description_t& description)
{
	const instance_t instance
	(
		description->origin,
		description->direction,
		description->up
	);
	return std::move(instance);
}

}
}
}
