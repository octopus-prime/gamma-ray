/*
 * test_interval.cpp
 *
 *  Created on: 06.03.2015
 *      Author: mike_gresens
 */

#include "math/interval.hpp"
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

typedef typename interval_t::base_type value_t;

BOOST_AUTO_TEST_SUITE(test_interval)

template <typename Function>
static void
test(const std::vector<value_t>& roots)
{
	Function f;
	for (const auto root : roots)
		BOOST_CHECK_CLOSE(boost::math::get<0>(f(root)) + 1.f, 1.f, 1e-4f);
}

class polynomial
{
public:
	template <typename Value>
	boost::math::tuple<Value, Value>
	operator()(const Value& x) const
	{
		return boost::math::make_tuple
		(
			(Value(1) * x + Value(5)) * x + Value(6),	// f(x)
			 Value(2) * x + Value(5)					// f'(x)
		);
	}
};

BOOST_AUTO_TEST_CASE(test_polynomial)
{
	const solver_t<polynomial> find;
	std::vector<value_t> roots;
	find(interval_t(-10, -10), std::back_inserter(roots));

	test<polynomial>(roots);
}

class trigonometrical
{
public:
	template <typename Value>
	boost::math::tuple<Value, Value>
	operator()(const Value& x) const
	{
		using namespace std;
		using namespace boost::numeric;

		return boost::math::make_tuple
		(
			sin(x),	// f(x)
			cos(x)	// f'(x)
		);
	}
};

BOOST_AUTO_TEST_CASE(test_trigonometrical)
{
	const solver_t<trigonometrical> find;
	std::vector<value_t> roots;
	find(interval_t(-10, -10), std::back_inserter(roots));

	test<trigonometrical>(roots);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
