/*
 * parser.cpp
 *
 *  Created on: 18.02.2015
 *      Author: mike_gresens
 */

#include <parsing/transformation/parser.hpp>
#include <parsing/transformation/transformation.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::texture::description_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::surface::description_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::description_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::texture::noise::description_t>;

namespace transformation {

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(),
	_transformable(),
	_texture(descriptions),
	_surface(descriptions),
	_object(descriptions),
	_vector3(descriptions),
	_noise(descriptions)
{
	_description =
			_transformable										[qi::_a = qi::_1]
			>
			+(
				qi::lit('.')
				>
				(
					_transformation(std::string("translate"))	[px::bind(translate, qi::_a, qi::_1)]
					|
					_transformation(std::string("rotate"))		[px::bind(rotate, qi::_a, qi::_1)]
					|
					_transformation(std::string("scale"))		[px::bind(scale, qi::_a, qi::_1)]
				)
			)
	;

	_transformation =
			qi::lit(qi::_r1) > qi::lit('(') > _vector3 > qi::lit(')')
	;

	_transformable =
			_texture
			|
			_surface
			|
			_object
			|
			_noise
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
