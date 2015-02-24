/*
 * test_polynomial.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include <math/polynomial.hpp>
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

BOOST_AUTO_TEST_SUITE(test_polynomial)

constexpr polynomial2_t polynomial1 {{+1, +2, -3}};
constexpr polynomial2_t polynomial2 {{+4, -5, +6}};
constexpr double scalar = -3;

BOOST_AUTO_TEST_CASE(test_foo)
{
	BOOST_CHECK_EQUAL(+polynomial1, polynomial1);
}

BOOST_AUTO_TEST_CASE(test_negate)
{
	constexpr polynomial2_t expected {{-1, -2, +3}};
	BOOST_CHECK_EQUAL(-polynomial1, expected);
}

BOOST_AUTO_TEST_CASE(test_plus)
{
	constexpr polynomial2_t expected {{+5, -3, +3}};
	BOOST_CHECK_EQUAL(polynomial1 + polynomial2, expected);
}

BOOST_AUTO_TEST_CASE(test_minus)
{
	constexpr polynomial2_t expected {{-3, +7, -9}};
	BOOST_CHECK_EQUAL(polynomial1 - polynomial2, expected);
}

BOOST_AUTO_TEST_CASE(test_multiplies_scalar)
{
	constexpr polynomial2_t expected {{-3, -6, +9}};
	BOOST_CHECK_EQUAL(polynomial1 * scalar, expected);
	BOOST_CHECK_EQUAL(scalar * polynomial1, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_scalar)
{
	constexpr polynomial2_t expected {{+1.0 / -3.0, +2.0 / -3.0, -3.0 / -3.0}};
	BOOST_CHECK_EQUAL(polynomial1 / scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_multiplies_polynomial)
{
	constexpr polynomial4_t expected {{+4, +3, -16, +27, -18}};
	BOOST_CHECK_EQUAL(polynomial1 * polynomial2, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_polynomial)
{
	constexpr polynomial4_t divisor {{+4, +3, -16, +27, -18}};
	BOOST_CHECK_EQUAL(divisor / polynomial2, polynomial1);
}

BOOST_AUTO_TEST_CASE(test_evaluate)
{
	BOOST_CHECK_EQUAL(evaluate(polynomial1, -2), -15);
	BOOST_CHECK_EQUAL(evaluate(polynomial1, -1),  -4);
	BOOST_CHECK_EQUAL(evaluate(polynomial1,  0),  +1);
	BOOST_CHECK_EQUAL(evaluate(polynomial1, +1),   0);
	BOOST_CHECK_EQUAL(evaluate(polynomial1, +2),  -7);
}

/*
BOOST_AUTO_TEST_CASE(test_differentiate)
{
	constexpr polynomial1_t expected {{+2, -6}};
	BOOST_CHECK_EQUAL(differentiate(polynomial1), expected);
}
*/
BOOST_AUTO_TEST_SUITE_END()

}
}
