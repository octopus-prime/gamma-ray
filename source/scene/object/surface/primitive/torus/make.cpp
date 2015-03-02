/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/torus/make.hpp>
#include <scene/object/surface/primitive/torus/model.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<torus::model>;

namespace torus {

surface::instance_t
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(trace) << "Make torus" << std::endl;

	return primitive::make<model>
	(
		description->transformation,
		description->major,
		description->minor
	);
}

}
}
}
}
}
}
