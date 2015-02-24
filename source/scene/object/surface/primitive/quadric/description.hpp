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
namespace quadric {

struct basic_description_t
{
	vector3_t square;
	vector3_t mixed;
	vector3_t absolute;
	float constant;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
