/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <parsing/identifier/parser.hpp>
#include <boost/spirit/include/qi.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;

namespace identifier {

static const std::string NAME("Identifier");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser()
:
	parser::base_type(_identifier),
	_identifier(NAME)
{
	_identifier =
			_string - _keywords;

	_string =
			qi::lexeme[qi::alpha > *qi::alnum];

	_keywords.add
			("include")
			("render")
			("Camera")
			("Light")
			("Scene")
			("Object")
			("Sphere")
			("Cube")
			("Mesh")
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
