/*
 * copy.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/light/copy.hpp>
#include <boost/make_shared.hpp>

namespace rt {
namespace scene {
namespace light {

description_t
copy(const description_t& description)
{
	return boost::make_shared<basic_description_t>(*description);
}

}
}
}
