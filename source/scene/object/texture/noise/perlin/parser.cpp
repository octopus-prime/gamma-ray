/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/texture/noise/perlin/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::texture::noise::perlin::basic_description_t,
	(boost::optional<double>, 	frequency)
	(boost::optional<double>, 	lacunarity)
	(boost::optional<int>, 		octaveCount)
	(boost::optional<double>, 	persistence)
	(boost::optional<int>, 		seed)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;

}

namespace scene {
namespace object {
namespace texture {
namespace noise {
namespace perlin {

static const std::string NAME("Perlin");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_options()
{
	static const auto make = [](const basic_description_t& description) -> description_t
	{
		return boost::make_shared<basic_description_t>(description);
	};

	_description =
			_basic_description [qi::_val = px::bind(make, qi::_1)]
	;

	_basic_description =
			qi::lit(NAME) > qi::lit('{')
			>
			-_options
			>
			qi::lit('}')
	;

	_options =
			(qi::lit("frequency") > qi::lit('=') > qi::double_)
			^
			(qi::lit("lacunarity") > qi::lit('=') > qi::double_)
			^
			(qi::lit("octaveCount") > qi::lit('=') > qi::int_)
			^
			(qi::lit("persistence") > qi::lit('=') > qi::double_)
			^
			(qi::lit("seed") > qi::lit('=') > qi::int_)
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
}
