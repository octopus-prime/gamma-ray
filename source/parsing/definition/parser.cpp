/*
 * parser.cpp
 *
 *  Created on: 18.02.2015
 *      Author: mike_gresens
 */

#include <parsing/definition/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <parsing/reader.hpp>
#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class filename::parser<iterator_t, skipper::parser<iterator_t>>;
extern template class transformation::parser<iterator_t, skipper::parser<iterator_t>>;
extern template class variable::set::parser<iterator_t, skipper::parser<iterator_t>>;

extern template bool read(const std::string& file, definition::parser<iterator_t, skipper::parser<iterator_t>>&, qi::unused_type&);

namespace definition {

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(variable::descriptions_t& descriptions)
:
	parser::base_type(_foo),
	_foo(),
	_include(),
	_def(descriptions),
	_filename(),
	_include_file(),
	_transformation(descriptions)
{
	const auto include = [&descriptions](const std::string& file) -> bool
	{
		parser<iterator_t, skipper::parser<iterator_t>> parser(descriptions);
		qi::unused_type unused;
		return read(file, parser, unused);
	};

	_foo =
			*(_include | _transformation | _def)
	;

	_include =
			qi::lit("include") > _include_file
	;

	_include_file =
			_filename  [qi::_pass = px::bind(include, qi::_1)]
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
