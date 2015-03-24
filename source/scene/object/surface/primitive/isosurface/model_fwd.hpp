/*
 * model_fwd.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <boost/function.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace isosurface {

typedef boost::function<float (vector3_t)> function_t;

}
}
}
}
}
}
