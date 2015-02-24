/*
 * parser.hpp
 *
 *  Created on: 18.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <parsing/filename/parser.hpp>
#include <parsing/transformation/parser.hpp>
#include <parsing/variable/set/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace definition {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper>
{
public:
	parser(variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper> _foo;
	qi::rule<Iterator, Skipper> _include;
	variable::set::parser<Iterator, Skipper> _def;
	filename::parser<Iterator, Skipper> _filename;
	qi::rule<Iterator, Skipper> _include_file;
	transformation::parser<Iterator, Skipper> _transformation;
};

}
}
}
