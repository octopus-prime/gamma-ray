/*
 * transformation.cpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#include <parsing/transformation/transformation.hpp>
#include <math/transformation.hpp>

namespace rt {
namespace parsing {
namespace transformation {

class transformation_visitor
:
	public boost::static_visitor<>
{
public:
	transformation_visitor(const matrix44_t& transformation)
	:
		_transformation(transformation)
	{
	}

	template <typename CSG>
	result_type operator()(const scene::object::surface::csg::description_t<CSG>& transformable) const
	{
		this->operator()(transformable->surface1);
		this->operator()(transformable->surface2);
	}

	template <typename Primitive>
	result_type operator()(const scene::object::surface::primitive::description_t<Primitive>& transformable) const
	{
		transformable->transformation *= _transformation;
	}

	result_type operator()(const scene::object::surface::description_t& transformable) const
	{
		boost::apply_visitor(*this, transformable);
	}

	result_type operator()(const scene::object::texture::description_t& transformable) const
	{
//		boost::apply_visitor(*this, transformable);
	}

	result_type operator()(const scene::object::description_t& transformable) const
	{
		this->operator()(transformable->surface);
		this->operator()(transformable->texture);
	}

private:
	const matrix44_t& _transformation;
};

void translate(const description_t& transformable, const vector3_t& translation)
{
	const matrix44_t transformation = rt::translate(translation);
	const transformation_visitor visitor(transformation);
	boost::apply_visitor(visitor, transformable);
}

void rotate(const description_t& transformable, const vector3_t& rotation)
{
	const matrix44_t transformation = rt::rotate(rotation);
	const transformation_visitor visitor(transformation);
	boost::apply_visitor(visitor, transformable);
}

void scale(const description_t& transformable, const vector3_t& scaling)
{
	const matrix44_t transformation = rt::scale(scaling);
	const transformation_visitor visitor(transformation);
	boost::apply_visitor(visitor, transformable);
}

}
}
}
