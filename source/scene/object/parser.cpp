/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::basic_description_t,
	(rt::scene::object::surface::description_t, surface)
	(rt::scene::object::texture::description_t, texture)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::description_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;

}

namespace scene {
namespace object {

extern template class surface::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

static const std::string NAME("Object");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_surface(descriptions),
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
				qi::lit("surface") > qi::lit('=') > _surface
				>
				qi::lit("texture") > qi::lit('=') > _vector3
				>
				qi::lit('}')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
