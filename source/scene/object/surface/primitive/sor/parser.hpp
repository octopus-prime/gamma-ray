/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/sor/description.hpp>
#include <parsing/vector/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace sor {

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
	qi::rule<Iterator, Skipper, std::vector<vector2_t>()> _points;
	parsing::vector::parser<Iterator, Skipper, 2> _vector2;
};

}
}
}
}
}
}
