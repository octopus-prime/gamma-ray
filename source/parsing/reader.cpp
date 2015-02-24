/*
 * reader.cpp
 *
 *  Created on: 07.02.2015
 *      Author: mike_gresens
 */

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>
#include <parsing/description/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <parsing/reader.hpp>
#include <fstream>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class description::parser<iterator_t, skipper::parser<iterator_t>>;
extern template class definition::parser<iterator_t, skipper::parser<iterator_t>>;

template <typename Parser, typename Attribute>
bool read(const std::string& file, Parser& parser, Attribute& attribute)
{
	std::clog << __func__ << " " << file << std::endl;
	std::ifstream stream(file);
	if (stream.fail())
		return false;

	stream.unsetf(std::ios::skipws);

	iterator_t begin((iterator_t::base_type(stream)));
	const iterator_t end;

	try
	{
		const bool success = qi::phrase_parse(begin, end, parser, skipper::parser<iterator_t>(), attribute);
		if (!success || begin != end)
			throw std::runtime_error(file + " parse failed.");
	}
	catch (const qi::expectation_failure<iterator_t>& failure)
	{
		const std::size_t line = boost::spirit::get_line(failure.first);
		const std::size_t column = boost::spirit::get_column(begin, failure.first);
		auto first = boost::spirit::get_line_start(begin, failure.first);
		if (*first == '\n') ++first;
		const auto last = std::find(first, end, '\n');
		std::string text(first, last);
		boost::algorithm::replace_all(text, "\t", "    ");
		const std::string arrow(column - 1, ' ');
		boost::format what("%s:%d,%d: expected %s\n%s\n%s^");
		what % file % line % column % failure.what_ % text % arrow;
		throw std::runtime_error(what.str());
	}

	return true;
}

template bool read(const std::string& file, definition::parser<iterator_t, skipper::parser<iterator_t>>&, qi::unused_type&);
template bool read(const std::string& file, description::parser<iterator_t, skipper::parser<iterator_t>>&, scene::description_t&);

}
}
