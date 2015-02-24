/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/cube/make.hpp>
#include <scene/object/surface/primitive/cube/model.hpp>
#include <scene/object/surface/primitive/impl.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class impl<cube::model>;

namespace cube {

base_t
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
