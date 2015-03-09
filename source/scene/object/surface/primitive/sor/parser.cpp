/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/primitive/sor/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::surface::primitive::sor::basic_description_t,
	(std::vector<rt::vector2_t>, points)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 2>;

}

namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace sor {

static const std::string NAME("SoR");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_points(),
	_vector2(descriptions)
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
			qi::lit("points") > qi::lit('=') > _points
			>
			qi::lit('}')
	;

	_points =
			qi::lit('[')
			>
			_vector2 % qi::lit(',')
			>
			qi::lit(']')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
}
