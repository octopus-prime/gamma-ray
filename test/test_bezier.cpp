/*
 * test_bezier.cpp
 *
 *  Created on: 06.03.2015
 *      Author: mike_gresens
 */

#include "math/bezier.hpp"
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

typedef typename bezier3_t::value_type point_t;
typedef typename point_t::value_type value_t;

BOOST_AUTO_TEST_SUITE(test_bezier)

constexpr point_t p0 {{+1, +1}};
constexpr point_t p1 {{+2, -2}};
constexpr point_t p2 {{+3, +3}};
constexpr point_t p3 {{+4, -4}};

constexpr bezier3_t bezier {{p0, p1, p2, p3}};

BOOST_AUTO_TEST_CASE(test_get)
{
	BOOST_CHECK_EQUAL(bezier[0], p0);
	BOOST_CHECK_EQUAL(bezier[1], p1);
	BOOST_CHECK_EQUAL(bezier[2], p2);
	BOOST_CHECK_EQUAL(bezier[3], p3);
}

BOOST_AUTO_TEST_CASE(test_evaluate)
{
	const auto polynomial_bezier = to_polynomial(bezier);
	constexpr point_t p1 {{+2, -0.07407414913177490234}};
	constexpr point_t p2 {{+3, -0.25925958156585693359}};

	BOOST_CHECK_EQUAL(evaluate(polynomial_bezier, 0/3.f), p0);
	BOOST_CHECK_EQUAL(evaluate(polynomial_bezier, 1/3.f), p1);
	BOOST_CHECK_EQUAL(evaluate(polynomial_bezier, 2/3.f), p2);
	BOOST_CHECK_EQUAL(evaluate(polynomial_bezier, 3/3.f), p3);
}

BOOST_AUTO_TEST_CASE(test_differentiate)
{
	constexpr bezier2_t expected {{ {{+4, -12}}, {{+4, +20}}, {{+4, -28}} }};

	BOOST_CHECK_EQUAL(differentiate(bezier), expected);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
