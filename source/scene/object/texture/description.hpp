/*
 * description.hpp
 *
 *  Created on: 14.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <boost/variant.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace object {
namespace texture {

typedef boost::variant<vector3_t> description_t;

}
}
}
}
