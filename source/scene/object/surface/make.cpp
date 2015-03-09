/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/make.hpp>
#include <scene/object/surface/primitive/sphere/make.hpp>
#include <scene/object/surface/primitive/quadric/make.hpp>
#include <scene/object/surface/primitive/cube/make.hpp>
#include <scene/object/surface/primitive/torus/make.hpp>
#include <scene/object/surface/primitive/mesh/make.hpp>
#include <scene/object/surface/primitive/text/make.hpp>
#include <scene/object/surface/primitive/plane/make.hpp>
#include <scene/object/surface/csg/difference/make.hpp>
#include <scene/object/surface/csg/intersection/make.hpp>
#include <scene/object/surface/csg/union/make.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {

class make_visitor
:
	public boost::static_visitor<boost::tuple<instance_t, box_t, std::size_t>>
{
public:
//	template <typename Description>
//	result_type operator()(const Description& description) const
//	{
//		return result_type();
//	}

	result_type operator()(const csg::difference::description_t& description) const
	{
		return csg::difference::make(description);
	}

	result_type operator()(const csg::intersection::description_t& description) const
	{
		return csg::intersection::make(description);
	}

	result_type operator()(const csg::union_::description_t& description) const
	{
		return csg::union_::make(description);
	}

	result_type operator()(const primitive::sphere::description_t& description) const
	{
		return primitive::sphere::make(description);
	}

	result_type operator()(const primitive::cube::description_t& description) const
	{
		return primitive::cube::make(description);
	}

	result_type operator()(const primitive::quadric::description_t& description) const
	{
		return primitive::quadric::make(description);
	}

	result_type operator()(const primitive::torus::description_t& description) const
	{
		return primitive::torus::make(description);
	}

	result_type operator()(const primitive::mesh::description_t& description) const
	{
		return primitive::mesh::make(description);
	}

	result_type operator()(const primitive::text::description_t& description) const
	{
		return primitive::text::make(description);
	}

	result_type operator()(const primitive::plane::description_t& description) const
	{
		return primitive::plane::make(description);
	}
};

boost::tuple<instance_t, box_t, std::size_t>
make(const description_t& description)
{
	return boost::apply_visitor(make_visitor(), description);
}

}
}
}
}
