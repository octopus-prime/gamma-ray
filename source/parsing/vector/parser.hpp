/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <math/vector.hpp>
#include <parsing/variable/get/parser.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace vector {

template <typename Iterator, typename Skipper, std::size_t N>
class parser
:
	public qi::grammar<Iterator, Skipper, basic_vector_t<N>()>
{
	class elements
	:
		public qi::grammar<Iterator, Skipper, basic_vector_t<N>()>
	{
	public:
		elements();

	private:
		qi::rule<Iterator, Skipper, basic_vector_t<N>()> _elements;
	};

public:
	parser(const variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, basic_vector_t<N>()> _vector;
	qi::rule<Iterator, Skipper, basic_vector_t<N>()> _term;
	qi::rule<Iterator, Skipper, basic_vector_t<N>()> _factor;
	variable::get::parser<Iterator, Skipper, basic_vector_t<N>> _variable;
	elements _elements;
};

}
}
}
