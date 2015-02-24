/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/camera/description.hpp>
#include <scene/camera/instance.hpp>

namespace rt {
namespace scene {
namespace camera {

instance_t
make(const description_t& description);

}
}
}
