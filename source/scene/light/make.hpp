/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/light/description.hpp>
#include <scene/light/instance.hpp>

namespace rt {
namespace scene {
namespace light {

instance_t
make(const description_t& description);

}
}
}
