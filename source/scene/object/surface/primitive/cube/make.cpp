/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/cube/make.hpp>
#include <scene/object/surface/primitive/cube/model.hpp>
#include <scene/object/surface/primitive/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<cube::model>;

namespace cube {

surface::instance_t
make(const description_t& description)
{
	return primitive::make<model>(description->transformation);
}

}
}
}
}
}
}
