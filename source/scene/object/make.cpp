/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/make.hpp>
#include <scene/object/surface/make.hpp>
#include <scene/object/texture/make.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace object {
/*
instance_t
make(const description_t& description)
{
	const instance_t instance
	(
		surface::make(description->surface).get<0>(),
		texture::make(description->texture)
	);
	return std::move(instance);
}
*/
boost::tuple<instance_t, box_t>
make(const description_t& description)
{
	const boost::tuple<surface::instance_t, box_t> surface = surface::make(description->surface);
	return boost::make_tuple
	(
		instance_t
		(
			surface.get<0>(),
			texture::make(description->texture)
		),
		surface.get<1>()
	);
/*
	const instance_t instance
	(
		surface::make(description->surface).get<0>(),
		texture::make(description->texture)
	);
	return std::move(instance);
*/
}

}
}
}
