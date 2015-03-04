/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/description.hpp>
#include <scene/object/instance.hpp>
#include <boost/tuple/tuple.hpp>

namespace rt {
namespace scene {
namespace object {

//instance_t
//make(const description_t& description);

boost::tuple<instance_t, box_t>
make(const description_t& description);

}
}
}
