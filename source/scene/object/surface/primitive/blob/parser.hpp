/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/blob/description.hpp>
#include <parsing/variable/description.hpp>
#include <parsing/vector/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace blob {

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
	qi::rule<Iterator, Skipper, std::vector<std::pair<vector3_t, float>>()> _carriers;
	qi::rule<Iterator, Skipper, std::pair<vector3_t, float>()> _carrier;
	parsing::vector::parser<Iterator, Skipper, 3> _vector3;
};

}
}
}
}
}
}
