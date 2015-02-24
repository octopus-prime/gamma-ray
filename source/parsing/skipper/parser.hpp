/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace skipper {

template <typename Iterator>
class parser
:
	public qi::grammar<Iterator>
{
public:
	parser();

private:
	qi::rule<Iterator> _skip;
	qi::rule<Iterator> _line;
	qi::rule<Iterator> _block;
};

}
}
}
