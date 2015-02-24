/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/quadric/make.hpp>
#include <scene/object/surface/primitive/quadric/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<quadric::model>;

namespace quadric {

surface::instance_t
make(const description_t& description)
{
	const matrix44_t matrix
	{{
		{{2 * description->square[0],	description->mixed[0],		description->mixed[1],		description->absolute[0]}},
		{{description->mixed[0],		2 * description->square[1],	description->mixed[2], 		description->absolute[1]}},
		{{description->mixed[1],		description->mixed[2],		2 * description->square[2],	description->absolute[2]}},
		{{description->absolute[0],		description->absolute[1],	description->absolute[2],	2 * description->constant}}
	}};
	return primitive::make<model>(description->transformation, std::move(0.5 * matrix));
}

}
}
}
}
}
}
