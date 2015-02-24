/*
 * parser.cpp
 *
 *  Created on: 18.02.2015
 *      Author: mike_gresens
 */

#include <parsing/description/parser.hpp>
#include <boost/spirit/include/qi.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class definition::parser<iterator_t, skipper::parser<iterator_t>>;

}

namespace scene {

extern template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}

namespace parsing {
namespace description {

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(variable::descriptions_t& descriptions)
:
	parser::base_type(_desc),
	_defs(descriptions),
	_scene(descriptions)
{
	_desc =
			qi::eps
			>
			_defs
			>
			qi::lit("render") > qi::lit('(') > _scene > qi::lit(')');
}

template class parser<iterator_t, skipper::parser<iterator_t>>;

}
}
}
