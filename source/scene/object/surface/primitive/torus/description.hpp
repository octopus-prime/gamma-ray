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
namespace torus {

struct basic_description_t
{
	float major;
	float minor;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
