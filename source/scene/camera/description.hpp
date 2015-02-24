/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <math/vector.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace camera {

struct basic_description_t
{
	vector3_t origin;
	vector3_t direction;
	vector3_t up = {{0, 1, 0}};
};

typedef boost::shared_ptr<basic_description_t> description_t;

}
}
}
