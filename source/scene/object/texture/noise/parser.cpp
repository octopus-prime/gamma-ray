/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/texture/noise/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::texture::noise::description_t>;

}

namespace scene {
namespace object {
namespace texture {
namespace noise {

extern template class generator::perlin::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class generator::billow::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class generator::cylinders::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

static const std::string NAME("Noise");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_term(),
	_factor(),
	_variable(descriptions),
	_generator(),
	_perlin(descriptions),
	_billow(descriptions),
	_cylinders(descriptions)
{
	static const auto make_add = [](const description_t& description1, const description_t& description2) -> description_t
	{
		return boost::make_shared<combiner::basic_description_t<combiner::add_tag>>(description1, description2);
	};

	static const auto make_mul = [](const description_t& description1, const description_t& description2) -> description_t
	{
		return boost::make_shared<combiner::basic_description_t<combiner::mul_tag>>(description1, description2);
	};

	_description =
			_term 											[qi::_val = qi::_1]
			>
			*(
					(qi::lit('+') > _term)					[qi::_val = px::bind(make_add, qi::_val, qi::_1)]
//					|
//					(qi::lit('-') > _term)					[px::bind(sub, qi::_val, qi::_1)]
			)
	;

	_term =
			_factor											[qi::_val = qi::_1]
			>
			*(
					(qi::lit('*') > _factor)				[qi::_val = px::bind(make_mul, qi::_val, qi::_1)]
//					|
//					(qi::lit('/') > _factor)				[px::bind(div, qi::_val, qi::_1)]
			)
	;

	_factor =
			_generator										[qi::_val = qi::_1]
			|
//			qi::double_										[qi::_val = qi::_1]
//			|
			(qi::lit('(') > _description > qi::lit(')'))	[qi::_val = qi::_1]
//			|
//			(qi::lit('+') > _factor)						[qi::_val = qi::_1]
//			|
//			(qi::lit('-') > _factor)						[qi::_val = px::bind(neg, qi::_1)]
			|
//			(qi::double_ > qi::lit('*') > _factor)			[qi::_val = px::bind(make_mul, qi::_1, qi::_2)]
//			|
			_variable										[qi::_val = qi::_1]
	;

	_generator =
			qi::double_
			|
			_perlin
			|
			_billow
			|
			_cylinders
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
