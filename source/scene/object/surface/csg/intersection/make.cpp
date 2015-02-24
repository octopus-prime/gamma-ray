/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/intersection/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/surface/csg/impl.hpp>
#include <scene/object/surface/csg/intersection/model.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

extern template class impl<intersection::model>;

namespace intersection {

base_t make(const description_t& description)
{
	return csg::make<model>
	(
		surface::make(description->surface1),
		surface::make(description->surface2)
	);
}

}
}
}
}
}
}
