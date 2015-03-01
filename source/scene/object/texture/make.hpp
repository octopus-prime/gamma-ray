/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/texture/description.hpp>
#include <scene/object/texture/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {

instance_t
make(const description_t& description);

}
}
}
}
