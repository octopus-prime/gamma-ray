/*
 * parser.hpp
 *
 *  Created on: 12.02.2015
 *      Author: mgresens
 */

#pragma once

#include <boost/spirit/include/qi_grammar.hpp>
#include <parsing/variable/description.hpp>
#include <parsing/identifier/parser.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace variable {
namespace get {

template <typename Iterator, typename Skipper, typename Variable>
class parser
:
	public qi::grammar<Iterator, Skipper, Variable()>
{
public:
	parser(const descriptions_t& descriptions);

private:
	qi::rule<Iterator, Skipper, Variable()> _description;
	identifier::parser<Iterator, Skipper> _identifier;
};

}
}
}
}
