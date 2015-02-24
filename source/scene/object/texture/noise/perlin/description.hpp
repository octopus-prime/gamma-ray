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
namespace perlin {

struct basic_description_t
{
	boost::optional<double> 	frequency;
	boost::optional<double> 	lacunarity;
	boost::optional<int> 		octaveCount;
	boost::optional<double> 	persistence;
	boost::optional<int> 		seed;
};

typedef boost::shared_ptr<basic_description_t> description_t;

}
}
}
}
}
}
