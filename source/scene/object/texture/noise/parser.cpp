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

extern template class perlin::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;
extern template class billow::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

static const std::string NAME("Noise");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
//	_term(),
//	_primitive(),
	_variable(descriptions),
	_perlin(descriptions),
	_billow(descriptions),
	_cylinders(descriptions)
{
	_description = _variable | _perlin | _billow | _cylinders;
//	static const auto make_union = [](const desc::noise_t& noise1, const desc::noise_t& noise2) -> desc::noise_t
//	{
//		return std::make_shared<desc::basic_csg_t<desc::csg_union_tag>>(noise1, noise2);
//	};
//
//	static const auto make_intersecation = [](const desc::noise_t& noise1, const desc::noise_t& noise2) -> desc::noise_t
//	{
//		return std::make_shared<desc::basic_csg_t<desc::csg_intersection_tag>>(noise1, noise2);
//	};
//
//	static const auto make_difference = [](const desc::noise_t& noise1, const desc::noise_t& noise2) -> desc::noise_t
//	{
//		return std::make_shared<desc::basic_csg_t<desc::csg_difference_tag>>(noise1, noise2);
//	};
//
//	_noise =
//			_term 									[qi::_val = qi::_1]
//			>
//			*(
//					(qi::lit('+') > _term)			[qi::_val = px::bind(make_union, qi::_val, qi::_1)]
//					|
//					(qi::lit('-') > _term)			[qi::_val = px::bind(make_difference, qi::_val, qi::_1)]
//					|
//					(qi::lit('^') > _term)			[qi::_val = px::bind(make_intersecation, qi::_val, qi::_1)]
//			)
//	;
//
//	_term =
//			_primitive
//			|
//			_variable
//			|
//			(qi::lit('(') > _noise > qi::lit(')'))
//	;
//
//	_primitive =
//			_sphere
//			|
//			_cube
//			|
//			_mesh
//	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
