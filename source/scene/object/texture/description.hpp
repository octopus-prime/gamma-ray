/*
 * description.hpp
 *
 *  Created on: 14.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <scene/object/texture/noise/description.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {

struct basic_description_t
{
	vector3_t pigment;	// TODO
	vector3_t bump;		// TODO
	noise::description_t ambient;
	noise::description_t diffuse;
	noise::description_t specular;
	noise::description_t shininess;
	noise::description_t reflection;
	noise::description_t transparency;
	noise::description_t refraction;
};

typedef boost::shared_ptr<basic_description_t> description_t;

}
}
}
}
