/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/texture/noise/description.hpp>
#include <scene/object/texture/noise/perlin/parser.hpp>
#include <scene/object/texture/noise/billow/parser.hpp>
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
//	qi::rule<iterator_t, skip_parser, desc::noise_t()> _term;
//	qi::rule<iterator_t, skip_parser, desc::noise_t()> _primitive;
	parsing::variable::get::parser<Iterator, Skipper, description_t> _variable;
	perlin::parser<Iterator, Skipper> _perlin;
	billow::parser<Iterator, Skipper> _billow;
};

}
}
}
}
}
