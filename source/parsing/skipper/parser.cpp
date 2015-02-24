/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mike_gresens
 */

#include <parsing/skipper/parser.hpp>
#include <boost/spirit/include/qi.hpp>

#include <parsing/iterator.hpp>

namespace rt {
namespace parsing {
namespace skipper {

static const std::string SKIP("Skip");

template <typename Iterator>
parser<Iterator>::parser()
:
	parser::base_type(_skip),
	_skip(SKIP)
{
	_skip =
			qi::space
			|
			_line
			|
			_block
	;

	_line =
			qi::lit("//") >> *(qi::char_ - qi::eol) >> qi::eol
	;

	_block =
			qi::lit("/*") >> *(qi::char_ - qi::lit("*/")) >> qi::lit("*/")
	;
}

template class parser<iterator_t>;

}
}
}
