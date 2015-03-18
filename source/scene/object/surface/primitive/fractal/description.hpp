/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/description_fwd.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace fractal {

struct basic_description_t
{
	std::size_t power;
	vector4_t constant;
	std::size_t iterations;
	float precision;
	vector4_t slice;
	float distance;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
