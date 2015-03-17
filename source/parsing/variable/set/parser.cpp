/*
 * parser.cpp
 *
 *  Created on: 12.02.2015
 *      Author: mgresens
 */

#include <parsing/keywords.hpp>
#include <parsing/variable/set/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

#include <scene/copy.hpp>
#include <scene/camera/copy.hpp>
#include <scene/light/copy.hpp>
#include <scene/object/copy.hpp>
#include <scene/object/surface/copy.hpp>
#include <scene/object/texture/copy.hpp>

#include <boost/make_shared.hpp>

namespace px = boost::phoenix;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;

namespace variable {
namespace set {

class copy_visitor
:
	public boost::static_visitor<description_t>
{
public:
	template <typename T>
	result_type operator()(const T& description) const
	{
		return description;
	}

	template <typename T>
	result_type operator()(const boost::shared_ptr<T>& description) const
	{
		return boost::make_shared<T>(*description);
	}
};

template <>
copy_visitor::result_type
copy_visitor::operator()(const scene::object::description_t& description) const
{
	return scene::object::copy(description);
}

template <>
copy_visitor::result_type
copy_visitor::operator()(const scene::object::surface::description_t& description) const
{
	return scene::object::surface::copy(description);
}

template <>
copy_visitor::result_type
copy_visitor::operator()(const scene::object::texture::description_t& description) const
{
	return scene::object::texture::copy(description);
}

template <>
copy_visitor::result_type
copy_visitor::operator()(const scene::description_t& description) const
{
	return scene::copy(description);
}
/*
static const boost::array<std::string, 13> TYPES
{{
	"Int",
	"Float",
	"String",
	"Vector2",
	"Vector3",
	"Vector4",
	"Scene",
	"Camera",
	"Light",
	"Object",
	"Surface",
	"Texture",
	"Noise"
}};
*/
template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(descriptions_t& descriptions)
:
	parser::base_type(_definition),
	_definition(std::string("VariableDefinition")),
	_keyword(),
	_identifier(),
	_check(std::string("UndefinedIdentifier")),
	_string(),
	_vector2(descriptions),
	_vector3(descriptions),
	_vector4(descriptions),
	_scene(descriptions),
	_camera(descriptions),
	_light(descriptions),
	_object(descriptions),
	_surface(descriptions),
	_texture(descriptions),
	_noise(descriptions),
	_variable
	{{
		qi::int_,
		qi::float_,
		_string,
		_vector2,
		_vector3,
		_vector4,
		_scene,
		_camera,
		_light,
		_object,
		_surface,
		_texture,
		_noise
	}}
{
	const auto check = [&descriptions](const std::string& identifier) -> bool
	{
		return descriptions.find(identifier) == descriptions.end();
	};

	const auto insert = [&descriptions](const std::string& identifier, const description_t& description)
	{
		descriptions.emplace(identifier, boost::apply_visitor(copy_visitor(), description));
	};

	_definition =
			_keyword			[qi::_a = qi::_1]
			>
			_identifier			[qi::_b = qi::_1]
			>
			_check (qi::_b)
			>
			qi::lit('=')
			>
			qi::lazy (*qi::_a)	[px::bind(insert, qi::_b, qi::_1)]
	;

	for (std::size_t i = 0; i < 13; ++i)
	{
		auto& type = keywords.at(i);
		auto& rule = _variable.at(i);

		rule.name(type);
		_keyword.add(type, &rule);
	}

	_check =
			qi::eps (px::bind(check, qi::_r1))
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
}
