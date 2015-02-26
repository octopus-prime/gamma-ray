/*
 * description.hpp
 *
 *  Created on: 16.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {
namespace noise {
namespace generator {
namespace cylinders {

struct basic_description_t
{
	boost::optional<double> 	frequency;
};

typedef boost::shared_ptr<basic_description_t> description_t;

}
}
}
}
}
}
}
