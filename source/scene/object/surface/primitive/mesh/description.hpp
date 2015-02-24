/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/description_fwd.hpp>
#include <string>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace mesh {

struct basic_description_t
{
	std::string file;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
