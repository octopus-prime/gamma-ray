/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/description.hpp>
#include <scene/object/surface/primitive/parser.hpp>
#include <parsing/variable/get/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace surface {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, description_t()>
{
public:
	parser(const parsing::variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, description_t()> _surface;
	qi::rule<Iterator, Skipper, description_t()> _term;
	primitive::parser<Iterator, Skipper> _primitive;
	parsing::variable::get::parser<Iterator, Skipper, description_t> _variable;
};

}
}
}
}
