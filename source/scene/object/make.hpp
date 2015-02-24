/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/description.hpp>
#include <scene/object/instance.hpp>

namespace rt {
namespace scene {
namespace object {

instance_t
make(const description_t& description);

}
}
}
