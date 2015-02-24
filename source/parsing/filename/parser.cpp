/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <parsing/filename/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/filesystem/operations.hpp>
#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;
namespace fs = boost::filesystem;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;

namespace filename {

static const std::string NAME("Filename");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser()
:
	parser::base_type(_filename),
	_filename(NAME),
	_string(),
	_check(std::string("ExistingFile"))
{
	static const auto check = [](const std::string& filename) -> bool
	{
		return fs::exists(filename);
	};

	_filename =
			_string			[qi::_a = qi::_1]
			>
			_check(qi::_a)	[qi::_val = qi::_a]
	;

	_string =
			qi::lexeme[qi::lit('"') > *(qi::char_ - qi::lit('"')) > qi::lit('"')]
	;

	_check =
			qi::eps (px::bind(check, qi::_r1))
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
