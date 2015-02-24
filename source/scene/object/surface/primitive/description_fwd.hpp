/*
 * description_fwd.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <math/matrix.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

template <typename Description>
struct basic_description_t
:
	public Description
{
	basic_description_t(const Description& description)
	:
		Description(description),
		transformation(identity<4>())
	{
	}

	matrix44_t transformation;
};

template <typename Description>
using description_t = boost::shared_ptr<basic_description_t<Description>>;

}
}
}
}
}
