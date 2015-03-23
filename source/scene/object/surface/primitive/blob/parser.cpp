/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/primitive/blob/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

typedef std::vector<std::pair<rt::vector3_t, float>> carriers_t;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::surface::primitive::blob::basic_description_t,
	(carriers_t, carriers)
	(float, threshold)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class vector::parser<iterator_t, skipper::parser<iterator_t>, 3>;

}

namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace blob {

static const std::string NAME("Blob");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_carriers(),
	_carrier(),
	_vector3(descriptions)
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
			qi::lit("carriers") > qi::lit('=') > _carriers
			>
			qi::lit("threshold") > qi::lit('=') > qi::float_
			>
			qi::lit('}')
	;

	_carriers =
			qi::lit('[')
			>
			_carrier % qi::lit(',')
			>
			qi::lit(']')
	;

	_carrier =
			qi::lit('{')
			>
			_vector3
			>
			qi::lit(',')
			>
			qi::float_
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
