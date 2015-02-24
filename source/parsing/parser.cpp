/*
 * parser.cpp
 *
 *  Created on: 07.02.2015
 *      Author: mike_gresens
 */

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>
#include <parsing/description/parser.hpp>
#include <parsing/reader.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class description::parser<iterator_t, skipper::parser<iterator_t>>;
extern template bool read(const std::string& file, description::parser<iterator_t, skipper::parser<iterator_t>>&, scene::description_t&);

scene::description_t
parse(const std::string& file)
{
	variable::descriptions_t descriptions;
	description::parser<iterator_t, skipper::parser<iterator_t>> parser(descriptions);
	scene::description_t scene;
	if (!read(file, parser, scene))
		throw std::runtime_error(file + " not found.");
	return scene;
}

}
}
