/*
 * test_polynomial_solve.cpp
 *
 *  Created on: 06.02.2013
 *      Author: mike_gresens
 */

#include <math/polynomial.hpp>
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

BOOST_AUTO_TEST_SUITE(test_polynomial_solve)

constexpr double margin = 1e-3;	// 0.001%
constexpr polynomial2_t no_roots {{1, 1, 1}};

template <typename polynomial_t, typename roots_t>
inline void
test_solve(const polynomial_t& polynomial, const std::size_t real_roots, roots_t&& roots)
{
	const auto roots_begin = roots.begin();
	const auto roots_end = solve(polynomial, roots_begin);
	BOOST_REQUIRE_EQUAL(std::distance(roots_begin, roots_end), real_roots);
	std::for_each
	(
		roots_begin, roots_end,
		[&polynomial](const double root)
		{
			BOOST_CHECK_CLOSE(evaluate(polynomial, root) + 1.0, 1.0, margin);
		}
	);
}

template <std::size_t N>
inline void
test_solve(const basic_polynomial_t<N>& polynomial, const std::size_t real_roots)
{
	test_solve
	(
		polynomial,
		real_roots,
		std::array<double, N - 1>()
	);

//	test_solve
//	(
//		polynomialN_t(polynomial.cbegin(), polynomial.cend()),
//		real_roots,
//		std::vector<double>(N - 1)
//	);
}

BOOST_AUTO_TEST_CASE(test_solve_1_0)
{
	const polynomial1_t polynomial {{1, 0}};
	test_solve(polynomial, 0);
}

BOOST_AUTO_TEST_CASE(test_solve_1_1)
{
	const polynomial1_t polynomial {{1, 1}};
	test_solve(polynomial, 1);
}

BOOST_AUTO_TEST_CASE(test_solve_2_0)
{
	test_solve(no_roots, 0);
}

BOOST_AUTO_TEST_CASE(test_solve_2_2)
{
	const polynomial2_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}};
	test_solve(polynomial, 2);
}

BOOST_AUTO_TEST_CASE(test_solve_3_1)
{
	const polynomial3_t polynomial = polynomial1_t {{1, 1}} * no_roots;
	test_solve(polynomial, 1);
}

BOOST_AUTO_TEST_CASE(test_solve_3_3)
{
	const polynomial3_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}};
	test_solve(polynomial, 3);
}

BOOST_AUTO_TEST_CASE(test_solve_4_0)
{
	const polynomial4_t polynomial = no_roots * no_roots;
	test_solve(polynomial, 0);
}

BOOST_AUTO_TEST_CASE(test_solve_4_2)
{
	const polynomial4_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * no_roots;
	test_solve(polynomial, 2);
}

BOOST_AUTO_TEST_CASE(test_solve_4_4)
{
	const polynomial4_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}};
	test_solve(polynomial, 4);
}

BOOST_AUTO_TEST_CASE(test_solve_5_1)
{
	const polynomial5_t polynomial = polynomial1_t {{1, 1}} * no_roots * no_roots;
	test_solve(polynomial, 1);
}

BOOST_AUTO_TEST_CASE(test_solve_5_3)
{
	const polynomial5_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * no_roots;
	test_solve(polynomial, 3);
}

BOOST_AUTO_TEST_CASE(test_solve_5_5)
{
	const polynomial5_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}} * polynomial1_t {{5, 1}};
	test_solve(polynomial, 5);
}

BOOST_AUTO_TEST_CASE(test_solve_6_0)
{
	const polynomial6_t polynomial = no_roots * no_roots * no_roots;
	test_solve(polynomial, 0);
}

BOOST_AUTO_TEST_CASE(test_solve_6_2)
{
	const polynomial6_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * no_roots * no_roots;
	test_solve(polynomial, 2);
}

BOOST_AUTO_TEST_CASE(test_solve_6_4)
{
	const polynomial6_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}} * no_roots;
	test_solve(polynomial, 4);
}

BOOST_AUTO_TEST_CASE(test_solve_6_6)
{
	const polynomial6_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}} * polynomial1_t {{5, 1}} * polynomial1_t {{6, 1}};
	test_solve(polynomial, 6);
}

BOOST_AUTO_TEST_CASE(test_solve_8_0)
{
	const polynomial8_t polynomial = no_roots * no_roots * no_roots * no_roots;
	test_solve(polynomial, 0);
}

BOOST_AUTO_TEST_CASE(test_solve_8_2)
{
	const polynomial8_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * no_roots * no_roots * no_roots;
	test_solve(polynomial, 2);
}

BOOST_AUTO_TEST_CASE(test_solve_8_4)
{
	const polynomial8_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}} * no_roots * no_roots;
	test_solve(polynomial, 4);
}

BOOST_AUTO_TEST_CASE(test_solve_8_6)
{
	const polynomial8_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}} * polynomial1_t {{5, 1}} * polynomial1_t {{6, 1}} * no_roots;
	test_solve(polynomial, 6);
}

BOOST_AUTO_TEST_CASE(test_solve_8_8)
{
	const polynomial8_t polynomial = polynomial1_t {{1, 1}} * polynomial1_t {{2, 1}} * polynomial1_t {{3, 1}} * polynomial1_t {{4, 1}} * polynomial1_t {{5, 1}} * polynomial1_t {{6, 1}} * polynomial1_t {{7, 1}} * polynomial1_t {{8, 1}};
	test_solve(polynomial, 8);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
