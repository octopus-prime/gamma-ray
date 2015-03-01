/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/texture/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::texture::basic_description_t,
	(rt::vector3_t, pigment)
	(rt::vector3_t, bump)
	(rt::scene::object::texture::noise::description_t, ambient)
	(rt::scene::object::texture::noise::description_t, diffuse)
	(rt::scene::object::texture::noise::description_t, specular)
	(rt::scene::object::texture::noise::description_t, shininess)
	(rt::scene::object::texture::noise::description_t, reflection)
	(rt::scene::object::texture::noise::description_t, transparency)
	(rt::scene::object::texture::noise::description_t, refraction)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::texture::description_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;

}

namespace scene {
namespace object {
namespace texture {

static const std::string NAME("Texture");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_noise(descriptions),
	_variable(descriptions),
	_vector3(descriptions)
{
	static const auto make = [](const basic_description_t& description) -> description_t
	{
		return boost::make_shared<basic_description_t>(description);
	};

	_description =
			_basic_description 	[qi::_val = px::bind(make, qi::_1)]
			|
			_variable			[qi::_val = qi::_1]
	;

	_basic_description =
				qi::lit(NAME) > qi::lit('{')
				>
				qi::lit("pigment") > qi::lit('=') > _vector3
				>
				qi::lit("bump") > qi::lit('=') > _vector3
				>
				qi::lit("ambient") > qi::lit('=') > _noise
				>
				qi::lit("diffuse") > qi::lit('=') > _noise
				>
				qi::lit("specular") > qi::lit('=') > _noise
				>
				qi::lit("shininess") > qi::lit('=') > _noise
				>
				qi::lit("reflection") > qi::lit('=') > _noise
				>
				qi::lit("transparency") > qi::lit('=') > _noise
				>
				qi::lit("refraction") > qi::lit('=') > _noise
				>
				qi::lit('}')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
