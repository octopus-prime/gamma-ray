/*
 * description.hpp
 *
 *  Created on: 16.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/texture/noise/perlin/description.hpp>
#include <scene/object/texture/noise/billow/description.hpp>
#include <scene/object/texture/noise/cylinders/description.hpp>
#include <boost/variant.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {
namespace noise {

typedef boost::variant
<
	perlin::description_t,
	billow::description_t,
	cylinders::description_t
> description_t;

}
}
}
}
}
