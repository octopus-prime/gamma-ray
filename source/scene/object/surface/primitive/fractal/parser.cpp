/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/primitive/fractal/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::surface::primitive::fractal::basic_description_t,
	(rt::vector4_t, constant)
	(std::size_t, iterations)
	(float, precision)
	(rt::vector4_t, slice)
	(float, distance)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 4>;

}

namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace fractal {

static const std::string NAME("Fractal");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_vector4(descriptions)
{
	static const auto make = [](const basic_description_t& description) -> description_t
	{
		return boost::make_shared<primitive::basic_description_t<basic_description_t>>(description);
	};

	_description =
			_basic_description [qi::_val = px::bind(make, qi::_1)]
	;

	_basic_description =
			qi::lit(NAME) > qi::lit('{')
			>
			qi::lit("constant") > qi ::lit('=') > _vector4
			>
			qi::lit("iterations") > qi ::lit('=') > qi::uint_
			>
			qi::lit("precision") > qi ::lit('=') > qi::float_
			>
			qi::lit("slice") > qi ::lit('=') > _vector4
			>
			qi::lit("distance") > qi ::lit('=') > qi::float_
			>
			qi::lit('}')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
}
