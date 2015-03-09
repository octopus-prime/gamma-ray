/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/description_fwd.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace sor {

struct basic_description_t
{
	std::vector<vector2_t> points;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
