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
#include <parsing/string/parser.hpp>
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
	public qi::grammar<Iterator, Skipper, qi::locals<qi::rule<Iterator, Skipper, description_t()>*, std::string>>
{
public:
	parser(descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, qi::locals<qi::rule<Iterator, Skipper, description_t()>*, std::string>> _definition;
	qi::symbols<char, qi::rule<Iterator, Skipper, description_t()>* > _keyword;
	identifier::parser<Iterator, Skipper> _identifier;
	qi::rule<Iterator, Skipper, void(std::string)> _check;

	string::parser<Iterator, Skipper> _string;
	vector::parser<Iterator, Skipper, 2> _vector2;
	vector::parser<Iterator, Skipper, 3> _vector3;
	vector::parser<Iterator, Skipper, 4> _vector4;
	scene::parser<Iterator, Skipper> _scene;
	scene::camera::parser<Iterator, Skipper> _camera;
	scene::light::parser<Iterator, Skipper> _light;
	scene::object::parser<Iterator, Skipper> _object;
	scene::object::surface::parser<Iterator, Skipper> _surface;
	scene::object::texture::parser<Iterator, Skipper> _texture;
	scene::object::texture::noise::parser<Iterator, Skipper> _noise;

	boost::array<qi::rule<Iterator, Skipper, description_t()>, 13> _variable;
};

}
}
}
}
