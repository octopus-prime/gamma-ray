/*
 * parser.cpp
 *
 *  Created on: 12.02.2015
 *      Author: mgresens
 */

#include <parsing/variable/get/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

namespace rt {
namespace parsing {

extern template class skipper::parser<iterator_t>;

namespace variable {
namespace get {

template <typename Iterator, typename Skipper, typename Variable>
parser<Iterator, Skipper, Variable>::parser(const descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(),
	_identifier()
{
	const auto get = [&descriptions](const std::string& identifier, Variable& value) -> bool
	{
		try
		{
			const description_t& description = descriptions.at(identifier);
			value = boost::get<Variable>(description);
			return true;
		}
		catch (const std::exception& exception)
		{
			return false;
		}
	};

	_description =
			_identifier	[qi::_pass = px::bind(get, qi::_1, qi::_val)]
	;
}

template class parser<iterator_t, skipper::parser<iterator_t>, vector2_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, vector3_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::description_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::camera::description_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::light::description_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::object::description_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::object::surface::description_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::object::texture::description_t>;
template class parser<iterator_t, skipper::parser<iterator_t>, scene::object::texture::noise::description_t>;

}
}
}
}
