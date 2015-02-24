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
namespace string {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, std::string()>
{
public:
	parser();

private:
	qi::rule<Iterator, Skipper, std::string()> _string;
};

}
}
}
