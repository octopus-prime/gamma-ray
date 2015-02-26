/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/texture/noise/description.hpp>
#include <scene/object/texture/noise/generator/perlin/parser.hpp>
#include <scene/object/texture/noise/generator/billow/parser.hpp>
#include <scene/object/texture/noise/generator/cylinders/parser.hpp>
#include <parsing/variable/get/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace texture {
namespace noise {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, description_t()>
{
public:
	parser(const parsing::variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, description_t()> _description;
	qi::rule<Iterator, Skipper, description_t()> _term;
	qi::rule<Iterator, Skipper, description_t()> _factor;
	parsing::variable::get::parser<Iterator, Skipper, description_t> _variable;
	qi::rule<Iterator, Skipper, description_t()> _generator;
	generator::perlin::parser<Iterator, Skipper> _perlin;
	generator::billow::parser<Iterator, Skipper> _billow;
	generator::cylinders::parser<Iterator, Skipper> _cylinders;
};

}
}
}
}
}
