/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/description_fwd.hpp>
#include <scene/object/surface/primitive/isosurface/model_fwd.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace isosurface {

struct basic_description_t
{
	function_t function;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
