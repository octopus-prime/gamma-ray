/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/primitive/mesh/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT
(
	rt::scene::object::surface::primitive::mesh::basic_description_t,
	(std::string, file)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;
extern template class filename::parser<iterator_t, skipper::parser<iterator_t>>;

}

namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace mesh {

static const std::string NAME("Mesh");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser()
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description(),
	_filename()
{
	static const auto make = [](const basic_description_t& description) -> description_t
	{
		return boost::make_shared<primitive::basic_description_t<basic_description_t>>(description);
	};

	_description =
			_basic_description [qi::_val = px::bind(make, qi::_1)]
	;

	_basic_description =
			qi::lit(NAME) > qi::lit('{')
			>
			qi::lit("file") > qi::lit('=') > _filename
			>
			qi::lit('}')
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
}
