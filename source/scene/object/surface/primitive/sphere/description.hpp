/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/description_fwd.hpp>
#include <boost/optional.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace sphere {

struct basic_description_t
{
	boost::optional<vector3_t> origin;
	boost::optional<float> radius;
};

typedef primitive::description_t<basic_description_t> description_t;

}
}
}
}
}
}
