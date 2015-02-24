/*
 * description.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/csg/description_fwd.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

template <typename csg_tag>
struct basic_description_t
{
	basic_description_t(const surface::description_t& surface1, const surface::description_t& surface2)
	:
		surface1(surface1),
		surface2(surface2)
	{
	}

	surface::description_t surface1;
	surface::description_t surface2;
};

}
}
}
}
}
