/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/description.hpp>
#include <scene/camera/parser.hpp>
#include <scene/light/parser.hpp>
#include <scene/object/parser.hpp>
#include <parsing/variable/get/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {

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
	camera::parser<Iterator, Skipper> _camera;
	qi::rule<Iterator, Skipper, light::descriptions_t()> _lights;
	light::parser<Iterator, Skipper> _light;
	qi::rule<Iterator, Skipper, object::descriptions_t()> _objects;
	object::parser<Iterator, Skipper> _object;
	parsing::variable::get::parser<Iterator, Skipper, description_t> _variable;
};

}
}
