/*
 * test_spline.cpp
 *
 *  Created on: 06.03.2015
 *      Author: mike_gresens
 */

#include "math/spline.hpp"
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

typedef vector2_t point_t;
typedef std::vector<point_t> points_t;

BOOST_AUTO_TEST_SUITE(test_spline)

constexpr point_t p0 {{+1, +1}};
constexpr point_t p1 {{+2, -2}};
constexpr point_t p2 {{+3, +3}};
constexpr point_t p3 {{+4, -4}};

static const points_t points {{p0, p1, p2, p3}};

BOOST_AUTO_TEST_CASE(test_evaluate)
{
	const auto spline = make_spline(points.begin(), points.end());

	BOOST_CHECK_CLOSE(evaluate(spline, p0[X]), p0[Y], 1e-5);
	BOOST_CHECK_CLOSE(evaluate(spline, p1[X]), p1[Y], 1e-5);
	BOOST_CHECK_CLOSE(evaluate(spline, p2[X]), p2[Y], 1e-5);
	BOOST_CHECK_CLOSE(evaluate(spline, p3[X]), p3[Y], 1e-5);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
