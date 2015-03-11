/*
 * test_interval.cpp
 *
 *  Created on: 06.03.2015
 *      Author: mike_gresens
 */

#include <math/interval.hpp>
#include <math/vector.hpp>
#include <boost/numeric/interval/io.hpp>
#include <boost/test/unit_test.hpp>

namespace rt {
namespace test {

typedef typename interval_t::base_type value_t;

BOOST_AUTO_TEST_SUITE(test_interval)

template <typename Function>
static inline void
test(const Function& f, const std::vector<value_t>& roots)
{
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
	polynomial f;
	const solver_t<polynomial> find(f);
	std::vector<value_t> roots;
	find(interval_t(-10, +10), std::back_inserter(roots), solver_all());

	test<polynomial>(f, roots);
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
	trigonometrical f;
	const solver_t<trigonometrical> find(f);
	std::vector<value_t> roots;
	find(interval_t(-10, +10), std::back_inserter(roots), solver_all());

	test<trigonometrical>(f, roots);
}


class superellipsoid
{
public:
	constexpr
	superellipsoid(const interval_t::base_type e, const interval_t::base_type n, const boost::array<interval_t::base_type, 3>& o, const boost::array<interval_t::base_type, 3>& d)
	:
		_e(1.f / e),
		_n(1.f / n),
		_o(o),
		_d(d)
	{
	}

	template <typename Value>
	boost::math::tuple<Value, Value>
	operator()(const Value& t) const
	{
		constexpr interval_t::base_type h = 1e-3;
		return boost::math::make_tuple
		(
			function(t),
			(function(t - Value(2) * h) - Value(8) * function(t - h) + Value(8) * function(t + h) - function(t + Value(2) * h)) / (Value(12) * h)
		);
	}

protected:
	template <typename Value>
	Value
	function(const Value& t) const
	{
		const Value x = Value(_o[X] + _d[X] * t);
		const Value y = Value(_o[Y] + _d[Y] * t);
		const Value z = Value(_o[Z] + _d[Z] * t);

		return power(power(x * x, _e) + power(z * z, _e), _n/_e) + power(y * y, _n) - Value(1);
	}

	float
	power(const float x, const float y) const
	{
		return std::pow(x, y);
	}

	interval_t
	power(const interval_t x, const float y) const
	{
		return interval_t(std::pow(x.lower(), y), std::pow(x.upper(), y));
	}

private:
	interval_t::base_type _e;
	interval_t::base_type _n;
	boost::array<interval_t::base_type, 3> _o;
	boost::array<interval_t::base_type, 3> _d;
};

BOOST_AUTO_TEST_CASE(test_superellipsoid)
{
//	const superellipsoid f(2.5, 2.5, {{0.3,0.3,-2}}, {{0,0,+1}});
	const superellipsoid f(0.2, 0.2, {{0.3,0.3,-2}}, {{0,0,+1}});
	const solver_t<superellipsoid> find(f, 1e-5);
	std::vector<value_t> roots;
	find(interval_t(-1, +4), std::back_inserter(roots), solver_all());

	BOOST_MESSAGE("roots = " << roots.size());
	test<superellipsoid>(f, roots);
	for (auto r : roots)
		BOOST_MESSAGE("root = " << r);
}

BOOST_AUTO_TEST_SUITE_END()

}
}
