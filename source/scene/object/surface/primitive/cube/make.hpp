/*
 * make.hpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/cube/description.hpp>
#include <scene/object/surface/instance.hpp>
#include <geo/box.hpp>
#include <boost/tuple/tuple.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace cube {

boost::tuple<surface::instance_t, box_t, std::size_t>
make(const description_t& description);

}
}
}
}
}
}
