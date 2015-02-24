/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::basic_description_t,
    (rt::scene::camera::description_t, camera)
    (rt::scene::light::descriptions_t, lights)
    (rt::scene::object::descriptions_t, objects)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::description_t>;

}

namespace scene {

extern template class camera::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class light::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class object::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

static const std::string NAME("Scene");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_camera(descriptions),
	_lights(),
	_light(descriptions),
	_objects(),
	_object(descriptions),
	_variable(descriptions)
{
	static const auto make = [](const basic_description_t& description) -> description_t
	{
		return boost::make_shared<basic_description_t>(description);
	};

	_description =
			_basic_description	[qi::_val = px::bind(make, qi::_1)]
			|
			_variable			[qi::_val = qi::_1]
	;

	_basic_description =
			qi::lit(NAME) > qi::lit('{')
			>
			qi::lit("camera") > qi::lit('=') > _camera
			>
			qi::lit("lights") > qi::lit('=') > _lights
			>
			qi::lit("objects") > qi::lit('=') > _objects
			>
			qi::lit('}')
	;

	_lights =
			qi::lit('[')
			>
			_light % qi::lit(',')
			>
			qi::lit(']')
	;

	_objects =
			qi::lit('[')
			>
			_object % qi::lit(',')
			>
			qi::lit(']')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
