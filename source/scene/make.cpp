/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/make.hpp>
#include <scene/camera/make.hpp>
#include <scene/light/make.hpp>
#include <scene/object/make.hpp>
#include <boost/range/algorithm/transform.hpp>

namespace rt {
namespace scene {

instance_t
make(const description_t& description)
{
	camera::instance_t camera = camera::make(description->camera);

	light::instances_t lights;
	boost::transform
	(
		description->lights,
		std::back_inserter(lights),
		light::make
	);

	object::instances_t objects;
	boost::transform
	(
		description->objects,
		std::back_inserter(objects),
		object::make
	);

	const instance_t instance
	(
		std::move(camera),
		std::move(lights),
		std::move(objects)
	);
	return std::move(instance);
}

}
}
