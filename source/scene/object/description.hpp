/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/description.hpp>
#include <scene/object/texture/description.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace object {

struct basic_description_t
{
	surface::description_t surface;
	texture::description_t texture;
};

typedef boost::shared_ptr<basic_description_t> description_t;

typedef std::vector<description_t> descriptions_t;

}
}
}
