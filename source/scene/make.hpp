/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/description.hpp>
#include <scene/instance.hpp>

namespace rt {
namespace scene {

boost::tuple<instance_t, std::size_t>
make(const description_t& description);

}
}
