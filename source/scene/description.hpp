/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/camera/description.hpp>
#include <scene/light/description.hpp>
#include <scene/object/description.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {

struct basic_description_t
{
	camera::description_t camera;
	light::descriptions_t lights;
	object::descriptions_t objects;
};

typedef boost::shared_ptr<basic_description_t> description_t;

}
}
