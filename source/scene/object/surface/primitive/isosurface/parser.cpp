/*
 * parser.cpp
 *
 *  Created on: 09.02.2015
 *      Author: mgresens
 */

#include <scene/object/surface/primitive/isosurface/parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/make_shared.hpp>

#include <parsing/iterator.hpp>
#include <parsing/skipper/parser.hpp>

namespace px = boost::phoenix;

typedef std::vector<std::pair<rt::vector3_t, float>> carriers_t;

BOOST_FUSION_ADAPT_STRUCT
(
    rt::scene::object::surface::primitive::isosurface::basic_description_t,
	(rt::scene::object::surface::primitive::isosurface::function_t, function)
)

namespace rt {

namespace parsing {

extern template class skipper::parser<iterator_t>;

}

namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace isosurface {

static const std::string NAME("Isosurface");

template <typename Iterator, typename Skipper>
parser<Iterator, Skipper>::parser(const parsing::variable::descriptions_t& descriptions)
:
	parser::base_type(_description),
	_description(NAME),
	_basic_description()
{
	static const auto make = [](const basic_description_t& description) -> description_t
	{
		return boost::make_shared<primitive::basic_description_t<basic_description_t>>(description);
	};

	static const auto add = [](function_t& f1, const function_t& f2){f1 = [f1,f2](const vector3_t& point){return f1(point) + f2(point);};};
	static const auto sub = [](function_t& f1, const function_t& f2){f1 = [f1,f2](const vector3_t& point){return f1(point) - f2(point);};};
	static const auto mul = [](function_t& f1, const function_t& f2){f1 = [f1,f2](const vector3_t& point){return f1(point) * f2(point);};};
	static const auto div = [](function_t& f1, const function_t& f2){f1 = [f1,f2](const vector3_t& point){return f1(point) / f2(point);};};
	static const auto neg = [](const function_t& f){return [f](const vector3_t& point){return -f(point);};};
	static const auto val = [](const float value){return [value](const vector3_t& point){return value;};};

	_description =
			_basic_description [qi::_val = px::bind(make, qi::_1)]
	;

	_basic_description =
			qi::lit(NAME) > qi::lit('{')
			>
			qi::lit("function") > qi::lit('=') > _expr
			>
			qi::lit('}')
	;

	_expr =
			_term 									[qi::_val = qi::_1]
			>
			*(
					(qi::lit('+') > _term)			[px::bind(add, qi::_val, qi::_1)]
					|
					(qi::lit('-') > _term)			[px::bind(sub, qi::_val, qi::_1)]
			)
	;

	_term =
			_factor									[qi::_val = qi::_1]
			>
			*(
					(qi::lit('*') > _factor)		[px::bind(mul, qi::_val, qi::_1)]
					|
					(qi::lit('/') > _factor)		[px::bind(div, qi::_val, qi::_1)]
			)
	;

	_factor =
			_value									[qi::_val = qi::_1]
			|
			_variable								[qi::_val = qi::_1]
			|
			_unary_function							[qi::_val = qi::_1]
			|
			_binary_function						[qi::_val = qi::_1]
			|
			(qi::lit('(') > _expr > qi::lit(')'))	[qi::_val = qi::_1]
			|
			(qi::lit('+') > _factor)				[qi::_val = qi::_1]
			|
			(qi::lit('-') > _factor)				[qi::_val = px::bind(neg, qi::_1)]
	;

	_unary_function =
			_unary									[qi::_a = qi::_1]
			>
			(qi::lit('(') > _expr > qi::lit(')'))	[qi::_val = px::bind(qi::_a, qi::_1)]
	;

	_binary_function =
			_binary									[qi::_a = qi::_1]
			>
			(qi::lit('(') > _expr > qi::lit(',') > _expr > qi::lit(')'))	[qi::_val = px::bind(qi::_a, qi::_1, qi::_2)]
	;

	_value =
			qi::float_								[qi::_val = px::bind(val, qi::_1)]
	;

	_unary.add
		("sqrt", [](const function_t& f){return [f](const vector3_t& point){return std::sqrt(f(point));};})
		("cbrt", [](const function_t& f){return [f](const vector3_t& point){return std::cbrt(f(point));};})
		("sin", [](const function_t& f){return [f](const vector3_t& point){return std::sin(f(point));};})
		("cos", [](const function_t& f){return [f](const vector3_t& point){return std::cos(f(point));};})
		("tan", [](const function_t& f){return [f](const vector3_t& point){return std::tan(f(point));};})
		("abs", [](const function_t& f){return [f](const vector3_t& point){return std::abs(f(point));};})
		("log", [](const function_t& f){return [f](const vector3_t& point){return std::log(f(point));};})
		("exp", [](const function_t& f){return [f](const vector3_t& point){return std::exp(f(point));};})
	;

	_binary.add
		("min", [](const function_t& f1, const function_t& f2){return [f1,f2](const vector3_t& point){return std::min(f1(point), f2(point));};})
		("max", [](const function_t& f1, const function_t& f2){return [f1,f2](const vector3_t& point){return std::max(f1(point), f2(point));};})
		("pow", [](const function_t& f1, const function_t& f2){return [f1,f2](const vector3_t& point){return std::pow(f1(point), f2(point));};})
	;

	_variable.add
		("x", [](const vector3_t& point){return point[0];})
		("y", [](const vector3_t& point){return point[1];})
		("z", [](const vector3_t& point){return point[2];})
	;
}

template class parser<parsing::iterator_t, parsing::skipper::parser<parsing::iterator_t>>;

}
}
}
}
}
}
