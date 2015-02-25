/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/torus/description.hpp>
#include <scene/object/surface/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace torus {

surface::instance_t
make(const description_t& description);

}
}
}
}
}
}
