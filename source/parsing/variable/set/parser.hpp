/*
 * parser.hpp
 *
 *  Created on: 12.02.2015
 *      Author: mgresens
 */

#pragma once

#include <parsing/variable/description.hpp>
#include <parsing/identifier/parser.hpp>
#include <parsing/vector/parser.hpp>
#include <scene/parser.hpp>
#include <scene/camera/parser.hpp>
#include <scene/light/parser.hpp>
#include <scene/object/parser.hpp>
#include <scene/object/surface/parser.hpp>
#include <scene/object/texture/parser.hpp>
#include <scene/object/texture/noise/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace variable {
namespace set {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, qi::locals<std::string>>
{
public:
	parser(descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, qi::locals<std::string>> _definition;
	qi::rule<Iterator, Skipper, void(std::string)> _check;
	qi::rule<Iterator, Skipper, description_t()> _variable;
	identifier::parser<Iterator, Skipper> _identifier;
	vector::parser<Iterator, Skipper, 3> _vector3;
	scene::parser<Iterator, Skipper> _scene;
	scene::camera::parser<Iterator, Skipper> _camera;
	scene::light::parser<Iterator, Skipper> _light;
	scene::object::parser<Iterator, Skipper> _object;
	scene::object::surface::parser<Iterator, Skipper> _surface;
	scene::object::texture::parser<Iterator, Skipper> _texture;
	scene::object::texture::noise::parser<Iterator, Skipper> _noise;
};

}
}
}
}
