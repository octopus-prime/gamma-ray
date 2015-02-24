/*
 * parser.hpp
 *
 *  Created on: 18.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <parsing/transformation/description.hpp>
#include <parsing/vector/parser.hpp>
#include <parsing/variable/get/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace transformation {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, qi::locals<description_t>>
{
public:
	parser(variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, qi::locals<description_t>> _description;
	qi::rule<Iterator, Skipper, description_t()> _transformable;
	qi::rule<Iterator, Skipper, vector3_t(std::string)> _transformation;
	variable::get::parser<Iterator, Skipper, scene::object::texture::description_t> _texture;
	variable::get::parser<Iterator, Skipper, scene::object::surface::description_t> _surface;
	variable::get::parser<Iterator, Skipper, scene::object::description_t> _object;
	vector::parser<Iterator, Skipper, 3> _vector3;
};

}
}
}
