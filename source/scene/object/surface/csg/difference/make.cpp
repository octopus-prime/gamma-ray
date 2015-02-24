/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/csg/difference/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/surface/csg/difference/model.hpp>
#include <scene/object/surface/csg/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace csg {

extern template class instance<difference::model>;

namespace difference {

surface::instance_t
make(const description_t& description)
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
