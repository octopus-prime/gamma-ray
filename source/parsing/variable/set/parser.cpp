/*
 * parser.cpp
 *
 *  Created on: 12.02.2015
 *      Author: mgresens
 */

#include <parsing/variable/set/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;

namespace variable {
namespace set {

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(descriptions_t& descriptions)
:
	parser::base_type(_definition),
	_definition(std::string("VariableDefinition")),
	_check(std::string("UndefinedIdentifier")),
	_variable(),
	_identifier(),
	_vector3(descriptions),
	_scene(descriptions),
	_camera(descriptions),
	_light(descriptions),
	_object(descriptions),
	_surface(descriptions),
	_noise(descriptions)
{
	const auto check = [&descriptions](const std::string& identifier) -> bool
	{
		return descriptions.find(identifier) == descriptions.end();
	};

	const auto insert = [&descriptions](const std::string& identifier, const description_t& description)
	{
		descriptions.emplace(identifier, description);
	};

	_definition =
			_identifier		[qi::_a = qi::_1]
			>
			_check(qi::_a)
			>
			qi::lit('=')
			>
			_variable		[px::bind(insert, qi::_a, qi::_1)]
	;

	_check =
			qi::eps (px::bind(check, qi::_r1))
	;

	_variable =
			_vector3
			|
			_object
			|
			_surface
			|
			_camera
			|
			_light
			|
			_scene
			|
			_noise
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
}
