/*
 * test_matrix.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include <math/matrix.hpp>
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

BOOST_AUTO_TEST_SUITE(test_matrix)

constexpr matrix44_t matrix1
{{
	{{ +1,  -2,  +3,  -4}},
	{{ -5,  +6,  -7,  +8}},
	{{ +9, +10, -11, -12}},
	{{-13, +14, +15, -16}}
}};
constexpr matrix44_t matrix2
{{
	{{+17, +18, -19, -20}},
	{{+21, +22, +23, +24}},
	{{-25, -26, -27, -28}},
	{{-29, +30, -31, +32}}
}};
constexpr vector4_t vector{{+1, +2, -3, -4}};
constexpr float scalar = -3;

BOOST_AUTO_TEST_CASE(test_get)
{
	BOOST_CHECK_EQUAL(matrix1[0][X], +1);
	BOOST_CHECK_EQUAL(matrix1[0][Y], -2);
	BOOST_CHECK_EQUAL(matrix1[0][Z], +3);
	BOOST_CHECK_EQUAL(matrix1[0][W], -4);
	BOOST_CHECK_EQUAL(matrix1[1][X], -5);
	BOOST_CHECK_EQUAL(matrix1[1][Y], +6);
	BOOST_CHECK_EQUAL(matrix1[1][Z], -7);
	BOOST_CHECK_EQUAL(matrix1[1][W], +8);
	BOOST_CHECK_EQUAL(matrix1[2][X], +9);
	BOOST_CHECK_EQUAL(matrix1[2][Y], +10);
	BOOST_CHECK_EQUAL(matrix1[2][Z], -11);
	BOOST_CHECK_EQUAL(matrix1[2][W], -12);
	BOOST_CHECK_EQUAL(matrix1[3][X], -13);
	BOOST_CHECK_EQUAL(matrix1[3][Y], +14);
	BOOST_CHECK_EQUAL(matrix1[3][Z], +15);
	BOOST_CHECK_EQUAL(matrix1[3][W], -16);
}

BOOST_AUTO_TEST_CASE(test_foo)
{
	BOOST_CHECK_EQUAL(+matrix1, matrix1);
}

BOOST_AUTO_TEST_CASE(test_negate)
{
	constexpr matrix44_t expected
	{{
		{{ -1,  +2,  -3,  +4}},
		{{ +5,  -6,  +7,  -8}},
		{{ -9, -10, +11, +12}},
		{{+13, -14, -15, +16}}
	}};
	BOOST_CHECK_EQUAL(-matrix1, expected);
}

BOOST_AUTO_TEST_CASE(test_plus_assign)
{
	constexpr matrix44_t expected
	{{
		{{+18, +16, -16, -24}},
		{{+16, +28, +16, +32}},
		{{-16, -16, -38, -40}},
		{{-42, +44, -16, +16}}
	}};
	matrix44_t matrix = matrix1;
	BOOST_CHECK_EQUAL(matrix += matrix2, expected);
}

BOOST_AUTO_TEST_CASE(test_plus)
{
	constexpr matrix44_t expected
	{{
		{{+18, +16, -16, -24}},
		{{+16, +28, +16, +32}},
		{{-16, -16, -38, -40}},
		{{-42, +44, -16, +16}}
	}};
	BOOST_CHECK_EQUAL(matrix1 + matrix2, expected);
}

BOOST_AUTO_TEST_CASE(test_minus)
{
	constexpr matrix44_t expected
	{{
		{{-16, -20, +22, +16}},
		{{-26, -16, -30, -16}},
		{{+34, +36, +16, +16}},
		{{+16, -16, +46, -48}}
	}};
	BOOST_CHECK_EQUAL(matrix1 - matrix2, expected);
}

BOOST_AUTO_TEST_CASE(test_minus_assign)
{
	constexpr matrix44_t expected
	{{
		{{-16, -20, +22, +16}},
		{{-26, -16, -30, -16}},
		{{+34, +36, +16, +16}},
		{{+16, -16, +46, -48}}
	}};
	matrix44_t matrix = matrix1;
	BOOST_CHECK_EQUAL(matrix -= matrix2, expected);
}

BOOST_AUTO_TEST_CASE(test_multilies_matrix)
{
	constexpr matrix44_t expected
	{{
		{{ +16, -224,  -22, -280}},
		{{ -16, +464, +174, +696}},
		{{+986, +308, +728,  -16}},
		{{+162, -796, +660, -336}}
	}};
	BOOST_CHECK_EQUAL(matrix1 * matrix2, expected);
}

BOOST_AUTO_TEST_CASE(test_multilies_vector_post)
{
	constexpr vector4_t expected {{+4, -4, +110, +34}};
	BOOST_CHECK_EQUAL(matrix1 * vector, expected);
}

BOOST_AUTO_TEST_CASE(test_multilies_vector_pre)
{
	constexpr vector4_t expected {{+16, -76, -38, +112}};
	BOOST_CHECK_EQUAL(vector * matrix1, expected);
}

BOOST_AUTO_TEST_CASE(test_multilies_scalar)
{
	constexpr matrix44_t expected
	{{
		{{ -3,  +6,  -9, +12}},
		{{+15, -18, +21, -24}},
		{{-27, -30, +33, +36}},
		{{+39, -42, -45, +48}}
	}};
	BOOST_CHECK_EQUAL(matrix1 * scalar, expected);
	BOOST_CHECK_EQUAL(scalar * matrix1, expected);
}

BOOST_AUTO_TEST_CASE(test_multilies_assign_scalar)
{
	constexpr matrix44_t expected
	{{
		{{ -3,  +6,  -9, +12}},
		{{+15, -18, +21, -24}},
		{{-27, -30, +33, +36}},
		{{+39, -42, -45, +48}}
	}};
	matrix44_t matrix = matrix1;
	BOOST_CHECK_EQUAL(matrix *= scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_scalar)
{
	constexpr matrix44_t expected
	{{
		{{ +1.f / -3.f,  -2.f / -3.f,  +3.f / -3.f,  -4.f / -3.f}},
		{{ -5.f / -3.f,  +6.f / -3.f,  -7.f / -3.f,  +8.f / -3.f}},
		{{ +9.f / -3.f, +10.f / -3.f, -11.f / -3.f, -12.f / -3.f}},
		{{-13.f / -3.f, +14.f / -3.f, +15.f / -3.f, -16.f / -3.f}}
	}};
	BOOST_CHECK_EQUAL(matrix1 / scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_divides_assign_scalar)
{
	constexpr matrix44_t expected
	{{
		{{ +1.f / -3.f,  -2.f / -3.f,  +3.f / -3.f,  -4.f / -3.f}},
		{{ -5.f / -3.f,  +6.f / -3.f,  -7.f / -3.f,  +8.f / -3.f}},
		{{ +9.f / -3.f, +10.f / -3.f, -11.f / -3.f, -12.f / -3.f}},
		{{-13.f / -3.f, +14.f / -3.f, +15.f / -3.f, -16.f / -3.f}}
	}};
	matrix44_t matrix = matrix1;
	BOOST_CHECK_EQUAL(matrix /= scalar, expected);
}

BOOST_AUTO_TEST_CASE(test_identity)
{
	constexpr matrix44_t expected
	{{
		{{1, 0, 0, 0}},
		{{0, 1, 0, 0}},
		{{0, 0, 1, 0}},
		{{0, 0, 0, 1}}
	}};
	BOOST_CHECK_EQUAL(identity<4>(), expected);
}

BOOST_AUTO_TEST_CASE(test_transpose)
{
	constexpr matrix44_t expected
	{{
		{{ +1,  -5,  +9, -13}},
		{{ -2,  +6, +10, +14}},
		{{ +3,  -7, -11, +15}},
		{{ -4,  +8, -12, -16}}
	}};
	BOOST_CHECK_EQUAL(transpose(matrix1), expected);
}


BOOST_AUTO_TEST_CASE(test_invert)
{
	constexpr matrix44_t expected
	{{
		{{ -17/18.f,	-61/153.f,	+7/306.f,	1/51.f}},
		{{ -29/36.f,	-161/612.f,	+13/612.f,	+11/204.f}},
		{{ -7/9.f,		-101/306.f,	-4/153.f,	+5/102.f}},
		{{ -2/3.f,		-11/51.f,	-5/204.f,	+1/68.f}}
	}};
	const auto result = invert(matrix1);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			BOOST_CHECK_CLOSE(result[i][j], expected[i][j], 1e-4);
}

BOOST_AUTO_TEST_CASE(test_invert_identity)
{
	const auto expected = identity<4>();
	const auto result = invert(expected);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			BOOST_CHECK_CLOSE(result[i][j], expected[i][j], 1e-4);
}

BOOST_AUTO_TEST_CASE(test_size)
{
	constexpr std::size_t expected = 4;
	BOOST_CHECK_EQUAL(matrix1.size(), expected);
}

BOOST_AUTO_TEST_CASE(test_equal_to)
{
	BOOST_CHECK_EQUAL(matrix1 == matrix1, true);
	BOOST_CHECK_EQUAL(matrix1 == matrix2, false);
}

BOOST_AUTO_TEST_CASE(test_not_equal_to)
{
	BOOST_CHECK_EQUAL(matrix1 != matrix1, false);
	BOOST_CHECK_EQUAL(matrix1 != matrix2, true);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
