/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/description.hpp>
#include <scene/object/surface/primitive/sphere/parser.hpp>
#include <scene/object/surface/primitive/cube/parser.hpp>
#include <scene/object/surface/primitive/mesh/parser.hpp>
#include <scene/object/surface/primitive/quadric/parser.hpp>
#include <scene/object/surface/primitive/torus/parser.hpp>
#include <scene/object/surface/primitive/text/parser.hpp>
#include <scene/object/surface/primitive/plane/parser.hpp>
#include <scene/object/surface/primitive/sor/parser.hpp>
#include <scene/object/surface/primitive/superellipsoid/parser.hpp>
#include <scene/object/surface/primitive/fractal/parser.hpp>
#include <scene/object/surface/primitive/blob/parser.hpp>
#include <scene/object/surface/primitive/isosurface/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, surface::description_t()>
{
public:
	parser(const parsing::variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, surface::description_t()> _description;
	sphere::parser<Iterator, Skipper> _sphere;
	cube::parser<Iterator, Skipper> _cube;
	mesh::parser<Iterator, Skipper> _mesh;
	quadric::parser<Iterator, Skipper> _quadric;
	torus::parser<Iterator, Skipper> _torus;
	text::parser<Iterator, Skipper> _text;
	plane::parser<Iterator, Skipper> _plane;
	sor::parser<Iterator, Skipper> _sor;
	superellipsoid::parser<Iterator, Skipper> _superellipsoid;
	fractal::parser<Iterator, Skipper> _fractal;
	blob::parser<Iterator, Skipper> _blob;
	isosurface::parser<Iterator, Skipper> _isosurface;
};

}
}
}
}
}
