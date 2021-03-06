/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/primitive/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;

}

namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class sphere::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class cube::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class mesh::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class quadric::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class torus::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class text::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class plane::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class sor::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class superellipsoid::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class fractal::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class blob::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class isosurface::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

static const std::string NAME("Primitive");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_sphere(descriptions),
	_cube(),
	_mesh(),
	_quadric(descriptions),
	_text(),
	_plane(),
	_sor(descriptions),
	_superellipsoid(descriptions),
	_fractal(descriptions),
	_blob(descriptions),
	_isosurface(descriptions)
{
	_description =
			_sphere
			|
			_cube
			|
			_mesh
			|
			_quadric
			|
			_torus
			|
			_text
			|
			_plane
			|
			_sor
			|
			_superellipsoid
			|
			_fractal
			|
			_blob
			|
			_isosurface
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
