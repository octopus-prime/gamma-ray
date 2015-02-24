/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

struct union_tag;
struct intersection_tag;
struct difference_tag;

template <typename csg_tag>
struct basic_description_t;

template <typename Description>
using description_t = boost::shared_ptr<basic_description_t<Description>>;

}
}
}
}
}
