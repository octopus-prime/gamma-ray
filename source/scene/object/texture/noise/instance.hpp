/*
 * instance.hpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <boost/function.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {
namespace noise {

typedef boost::function<double (vector3_t)> instance_t;

}
}
}
}
}
