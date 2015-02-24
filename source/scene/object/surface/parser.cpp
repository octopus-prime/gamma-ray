/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, scene::object::surface::description_t>;

}

namespace scene {
namespace object {
namespace surface {

extern template class primitive::parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

static const std::string NAME("surface");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_surface),
	_surface(NAME),
	_term(),
	_primitive(descriptions),
	_variable(descriptions)
{
	static const auto make_union = [](const description_t& surface1, const description_t& surface2) -> description_t
	{
		return boost::make_shared<csg::basic_description_t<csg::union_tag>>(surface1, surface2);
	};

	static const auto make_intersecation = [](const description_t& surface1, const description_t& surface2) -> description_t
	{
		return boost::make_shared<csg::basic_description_t<csg::intersection_tag>>(surface1, surface2);
	};

	static const auto make_difference = [](const description_t& surface1, const description_t& surface2) -> description_t
	{
		return boost::make_shared<csg::basic_description_t<csg::difference_tag>>(surface1, surface2);
	};

	_surface =
			_term 									[qi::_val = qi::_1]
			>
			*(
					(qi::lit('+') > _term)			[qi::_val = px::bind(make_union, qi::_val, qi::_1)]
					|
					(qi::lit('-') > _term)			[qi::_val = px::bind(make_difference, qi::_val, qi::_1)]
					|
					(qi::lit('&') > _term)			[qi::_val = px::bind(make_intersecation, qi::_val, qi::_1)]
			)
	;

	_term =
			_primitive
			|
			_variable
			|
			(qi::lit('(') > _surface > qi::lit(')'))
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
