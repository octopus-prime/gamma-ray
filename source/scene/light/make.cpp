/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/light/make.hpp>

namespace rt {
namespace scene {
namespace light {

instance_t
make(const description_t& description)
{
	const instance_t instance
	(
		description->origin,
		description->color
	);
	return std::move(instance);
}

}
}
}
