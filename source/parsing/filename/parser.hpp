/*
 * parser.hpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#pragma once

#include <boost/spirit/include/qi_grammar.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {
namespace filename {

template <typename Iterator, typename Skipper>
class parser
:
	public qi::grammar<Iterator, Skipper, std::string(), qi::locals<std::string>>
{
public:
	parser();

private:
	qi::rule<Iterator, Skipper, std::string(), qi::locals<std::string>> _filename;
	qi::rule<Iterator, Skipper, std::string()> _string;
	qi::rule<Iterator, Skipper, void(std::string)> _check;
};

}
}
}
