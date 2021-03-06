/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <scene/object/surface/primitive/isosurface/description.hpp>
#include <parsing/variable/description.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_symbols.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace isosurface {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, description_t()>
{
public:
	parser(const parsing::variable::descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, description_t()> _description;
	qi::rule<Iterator, Skipper, basic_description_t()> _basic_description;
	qi::rule<Iterator, Skipper, function_t()> _expr;
	qi::rule<Iterator, Skipper, function_t()> _term;
	qi::rule<Iterator, Skipper, function_t()> _factor;
	qi::rule<Iterator, Skipper, function_t()> _value;
	qi::rule<Iterator, Skipper, function_t(), qi::locals<boost::function<function_t (function_t)>>> _unary_function;
	qi::rule<Iterator, Skipper, function_t(), qi::locals<boost::function<function_t (function_t, function_t)>>> _binary_function;
	qi::symbols<char, function_t> _variable;
	qi::symbols<char, boost::function<function_t (function_t)>> _unary;
	qi::symbols<char, boost::function<function_t (function_t, function_t)>> _binary;
};

}
}
}
}
}
}
