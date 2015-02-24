/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/csg/difference/description.hpp>
#include <scene/object/surface/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {
namespace difference {

surface::instance_t
make(const description_t& description);

}
}
}
}
}
}
