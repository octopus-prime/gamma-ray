/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <math/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace light {

struct basic_description_t
{
	vector3_t origin;
	vector3_t color;
};

typedef boost::shared_ptr<basic_description_t> description_t;

typedef std::vector<description_t> descriptions_t;

}
}
}
