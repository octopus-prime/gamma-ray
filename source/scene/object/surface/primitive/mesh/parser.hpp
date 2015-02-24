/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/mesh/description.hpp>
#include <parsing/filename/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace mesh {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, description_t()>
{
public:
	parser();

private:
	qi::rule<Iterator, Skipper, description_t()> _description;
	qi::rule<Iterator, Skipper, basic_description_t()> _basic_description;
	parsing::filename::parser<Iterator, Skipper> _filename;
};

}
}
}
}
}
}
