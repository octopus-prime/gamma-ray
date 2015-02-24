/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/camera/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::camera::basic_description_t,
	(rt::vector3_t, origin)
	(rt::vector3_t, direction)
	(rt::vector3_t, up)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::camera::description_t>;

}

namespace scene {
namespace camera {

static const std::string NAME("Camera");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_variable(descriptions),
	_vector3(descriptions)
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
			qi::lit("origin") > qi::lit('=') > _vector3
			>
			qi::lit("direction") > qi::lit('=') > _vector3
			>
			-(qi::lit("up") > qi::lit('=') > _vector3)
			>
			qi::lit('}')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
