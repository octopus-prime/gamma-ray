/*
 * description.hpp
 *
 *  Created on: 14.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/description.hpp>
#include <scene/object/surface/csg/description_fwd.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {

typedef boost::variant
<
	primitive::sphere::description_t,
	primitive::cube::description_t,
	primitive::mesh::description_t,
	primitive::quadric::description_t,
	primitive::torus::description_t,
	primitive::text::description_t,
	primitive::plane::description_t,
	primitive::sor::description_t,
	primitive::superellipsoid::description_t,
	primitive::fractal::description_t,
	primitive::blob::description_t,
	primitive::isosurface::description_t,
	// TODO: more surface types here...
	boost::recursive_wrapper<csg::description_t<csg::union_tag>>,
	boost::recursive_wrapper<csg::description_t<csg::intersection_tag>>,
	boost::recursive_wrapper<csg::description_t<csg::difference_tag>>
> description_t;

}
}
}
}

#include <scene/object/surface/csg/description.hpp>
