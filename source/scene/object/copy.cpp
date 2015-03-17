/*
 * copy.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/copy.hpp>
#include <scene/object/surface/copy.hpp>
#include <scene/object/texture/copy.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace object {

description_t
copy(const description_t& description)
{
	basic_description_t object
	{
		surface::copy(description->surface),
		texture::copy(description->texture)
	};

	return boost::make_shared<basic_description_t>(std::move(object));
}

}
}
}
