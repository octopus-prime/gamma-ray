/*
 * test_vector.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include "math/vector.hpp"
#include "math/container.hpp"
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

typedef typename vector3_t::value_type value_t;

BOOST_AUTO_TEST_SUITE(test_vector)

constexpr vector3_t vector1 {{+1, +2, -3}};
constexpr vector3_t vector2 {{+4, -5, +6}};
constexpr value_t scalar = -3;

BOOST_AUTO_TEST_CASE(test_get)
{
	BOOST_CHECK_EQUAL(vector1[X], +1);
	BOOST_CHECK_EQUAL(vector1[Y], +2);
	BOOST_CHECK_EQUAL(vector1[Z], -3);
}

BOOST_AUTO_TEST_CASE(test_plus_assign)
{
	constexpr vector3_t expected {{+5, -3, +3}};
	vector3_t vector = vector1;
	BOOST_CHECK_EQUAL(vector += vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_minus_assign)
{
	constexpr vector3_t expected {{-3, +7, -9}};
	vector3_t vector = vector1;
	BOOST_CHECK_EQUAL(vector -= vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_multiplies_assign_vector)
{
	constexpr vector3_t expected {{+4, -10, -18}};
	vector3_t vector = vector1;
	BOOST_CHECK_EQUAL(vector &= vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_assign_vector)
{
	constexpr vector3_t expected {{value_t(+1) / value_t(+4), value_t(+2) / value_t(-5), value_t(-3) / value_t(+6)}};
	vector3_t vector = vector1;
	BOOST_CHECK_EQUAL(vector |= vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_multiplies_assign_scalar)
{
	constexpr vector3_t expected {{-3, -6, +9}};
	vector3_t vector = vector1;
	BOOST_CHECK_EQUAL(vector *= scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_assign_scalar)
{
	constexpr vector3_t expected {{value_t(+1) / value_t(-3), value_t(+2) / value_t(-3), value_t(-3) / value_t(-3)}};
	vector3_t vector = vector1;
	BOOST_CHECK_EQUAL(vector /= scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_foo)
{
	BOOST_CHECK_EQUAL(+vector1, vector1);
}

BOOST_AUTO_TEST_CASE(test_negate)
{
	constexpr vector3_t expected {{-1, -2, +3}};
	BOOST_CHECK_EQUAL(-vector1, expected);
}

BOOST_AUTO_TEST_CASE(test_plus)
{
	constexpr vector3_t expected {{+5, -3, +3}};
	BOOST_CHECK_EQUAL(vector1 + vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_minus)
{
	constexpr vector3_t expected {{-3, +7, -9}};
	BOOST_CHECK_EQUAL(vector1 - vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_dot)
{
	constexpr value_t expected = -24;
	BOOST_CHECK_EQUAL(vector1 * vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_multiplies_vector)
{
	constexpr vector3_t expected {{+4, -10, -18}};
	BOOST_CHECK_EQUAL(vector1 & vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_vector)
{
	constexpr vector3_t expected {{value_t(+1) / value_t(+4), value_t(+2) / value_t(-5), value_t(-3) / value_t(+6)}};
	BOOST_CHECK_EQUAL(vector1 | vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_multiplies_scalar)
{
	constexpr vector3_t expected {{-3, -6, +9}};
	BOOST_CHECK_EQUAL(vector1 * scalar, expected);
	BOOST_CHECK_EQUAL(scalar * vector1, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_scalar)
{
	constexpr vector3_t expected {{value_t(+1) / value_t(-3), value_t(+2) / value_t(-3), value_t(-3) / value_t(-3)}};
	BOOST_CHECK_EQUAL(vector1 / scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_cross)
{
	constexpr vector3_t expected {{-3, -18, -13}};
	BOOST_CHECK_EQUAL(vector1 % vector2, expected);
}

BOOST_AUTO_TEST_CASE(test_length)
{
	static const value_t expected = std::sqrt(value_t(14));
	BOOST_CHECK_EQUAL(length(vector1), expected);
}

BOOST_AUTO_TEST_CASE(test_normalize)
{
	static const value_t length = std::sqrt(value_t(14));
	static const vector3_t expected {{+1 / length, +2 / length, -3 / length}};
	BOOST_CHECK_EQUAL(normalize(vector1), expected);
}

BOOST_AUTO_TEST_CASE(test_size)
{
	constexpr std::size_t expected = 3;
	BOOST_CHECK_EQUAL(vector1.size(), expected);
}

BOOST_AUTO_TEST_CASE(test_equal_to)
{
	BOOST_CHECK_EQUAL(vector1 == vector1, true);
	BOOST_CHECK_EQUAL(vector1 == vector2, false);
}

BOOST_AUTO_TEST_CASE(test_not_equal_to)
{
	BOOST_CHECK_EQUAL(vector1 != vector1, false);
	BOOST_CHECK_EQUAL(vector1 != vector2, true);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
