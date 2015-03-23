/*
 * copy.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/copy.hpp>
#include <scene/camera/copy.hpp>
#include <scene/light/copy.hpp>
#include <scene/object/copy.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {

description_t
copy(const description_t& description)
{
	light::descriptions_t lights;
	for (const auto& desc : description->lights)
		lights.emplace_back(light::copy(desc));

	object::descriptions_t objects;
	for (const auto& desc : description->objects)
		objects.emplace_back(object::copy(desc));

	scene::basic_description_t object =
	{
		camera::copy(description->camera),
		std::move(lights),
		std::move(objects)
	};

	return boost::make_shared<scene::basic_description_t>(std::move(object));
}

}
}
