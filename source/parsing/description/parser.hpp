/*
 * parser.hpp
 *
 *  Created on: 18.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <parsing/variable/description.hpp>
#include <parsing/definition/parser.hpp>
#include <scene/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace description {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, scene::description_t()>
{
public:
	parser(variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, scene::description_t()> _desc;
	definition::parser<Iterator, Skipper> _defs;
	scene::parser<Iterator, Skipper> _scene;
};

}
}
}
