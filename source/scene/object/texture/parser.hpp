/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/texture/description.hpp>
#include <scene/object/texture/noise/parser.hpp>
#include <parsing/variable/get/parser.hpp>
#include <parsing/vector/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace texture {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, description_t()>
{
public:
	parser(const parsing::variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, description_t()> _description;
	qi::rule<Iterator, Skipper, basic_description_t()> _basic_description;
	qi::rule<Iterator, Skipper, basic_description_t()> _options;
	noise::parser<Iterator, Skipper> _noise;
	parsing::variable::get::parser<Iterator, Skipper, description_t> _variable;
	parsing::vector::parser<Iterator, Skipper, 3> _vector3;
};

}
}
}
}
