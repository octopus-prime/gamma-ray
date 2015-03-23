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
namespace blob {

struct basic_description_t
{
	std::vector<std::pair<vector3_t, float>> carriers;
	float threshold;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
