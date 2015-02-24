/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <parsing/vector/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/format.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, vector2_t>;
extern template class variable::get::parser<iterator_t, skipper::parser<iterator_t>, vector3_t>;

namespace vector {

template <>
parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>, 2>::elements::elements()
:
	elements::base_type(_elements),
	_elements()
{
	_elements =
			qi::lit('<')
			>
			qi::float_ [px::at(qi::_val, 0) = qi::_1]
			>
			qi::lit(',')
			>
			qi::float_ [px::at(qi::_val, 1) = qi::_1]
			>
			qi::lit('>')
	;
}

template <>
parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>, 3>::elements::elements()
:
	elements::base_type(_elements),
	_elements()
{
	_elements =
			qi::lit('<')
			>
			qi::float_ [px::at(qi::_val, 0) = qi::_1]
			>
			qi::lit(',')
			>
			qi::float_ [px::at(qi::_val, 1) = qi::_1]
			>
			qi::lit(',')
			>
			qi::float_ [px::at(qi::_val, 2) = qi::_1]
			>
			qi::lit('>')
	;
}

template <typename Iterator, typename Skipper, std::size_t N>
parser<Iterator, Skipper, N>::parser(const variable::descriptions_t& descriptions)
:
	parser::base_type(_vector),
	_vector((boost::format("Vector%d") % N).str()),
	_term(),
	_factor(),
	_variable(descriptions),
	_elements()
{
	static const auto add = [](basic_vector_t<N>& vector1, const basic_vector_t<N>& vector2){vector1 += vector2;};
	static const auto sub = [](basic_vector_t<N>& vector1, const basic_vector_t<N>& vector2){vector1 -= vector2;};
	static const auto mul = [](basic_vector_t<N>& vector, const float scalar){vector *= scalar;};
	static const auto div = [](basic_vector_t<N>& vector, const float scalar){vector /= scalar;};
	static const auto neg = [](const basic_vector_t<N>& vector){return -vector;};

	_vector =
			_term 									[qi::_val = qi::_1]
			>
			*(
					(qi::lit('+') > _term)			[px::bind(add, qi::_val, qi::_1)]
					|
					(qi::lit('-') > _term)			[px::bind(sub, qi::_val, qi::_1)]
			)
	;

	_term =
			_factor									[qi::_val = qi::_1]
			>
			*(
					(qi::lit('*') > qi::float_)		[px::bind(mul, qi::_val, qi::_1)]
					|
					(qi::lit('/') > qi::float_)		[px::bind(div, qi::_val, qi::_1)]
			)
	;

	_factor =
			_elements								[qi::_val = qi::_1]
			|
			_variable								[qi::_val = qi::_1]
			|
			(qi::lit('(') > _vector > qi::lit(')'))	[qi::_val = qi::_1]
			|
			(qi::lit('+') > _factor)				[qi::_val = qi::_1]
			|
			(qi::lit('-') > _factor)				[qi::_val = px::bind(neg, qi::_1)]
			|
			(qi::float_ > qi::lit('*') > _factor)	[qi::_val = qi::_2, px::bind(mul, qi::_val, qi::_1)]
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>, 2>;
template class parser<iterator_t, skipper::parser<iterator_t>, 3>;

}
}
}
