/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <parsing/string/parser.hpp>
#include <boost/spirit/include/qi.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;

namespace string {

static const std::string NAME("String");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser()
:
	parser::base_type(_string),
	_string(NAME)
{
	_string =
			qi::lexeme[qi::lit('"') > *(qi::char_ - qi::lit('"')) > qi::lit('"')];
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
