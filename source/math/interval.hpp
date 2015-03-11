/*
 * interval.hpp
 *
 *  Created on: 11.03.2015
 *      Author: mike_gresens
 */

#pragma once

#include <boost/numeric/interval.hpp>
#include <boost/math/tools/roots.hpp>

namespace rt {

template <typename Value>
using basic_interval_t = boost::numeric::interval
<
	Value,
	boost::numeric::interval_lib::policies
	<
		boost::numeric::interval_lib::save_state
		<
			boost::numeric::interval_lib::rounded_transc_exact<Value>
		>,
		boost::numeric::interval_lib::checking_base<Value>
	>
>;

typedef basic_interval_t<float> interval_t;	// Change to double for exacter results...

struct solver_lowest {};
struct solver_all {};

template <typename Function>
class solver_t
{
public:
	solver_t(const Function& function, const interval_t::base_type eps = std::numeric_limits<interval_t::base_type>::epsilon())
	:
		_function(function),
		_eps(eps)
	{
	}

	template <typename Iterator>
	Iterator
	operator()(const interval_t& interval, Iterator iterator, solver_all) const
	{
		const auto result = _function(interval);

		if (boost::math::get<0>(result).lower() * boost::math::get<0>(result).upper() > 0)
			return iterator;

		if (boost::math::get<1>(result).lower() * boost::math::get<1>(result).upper() > 0)
		{
			if (boost::math::get<0>(_function(interval.lower())) * boost::math::get<0>(_function(interval.upper())) <= 0)
				*iterator++ = solve(interval);
			return iterator;
		}

		if (boost::numeric::width(interval) <= _eps)
			return iterator;

		const auto median = boost::numeric::median(interval);
		iterator = (*this)(interval_t(interval.lower(), median), iterator, solver_all());
		iterator = (*this)(interval_t(median, interval.upper()), iterator, solver_all());

		return iterator;
	}

	template <typename Iterator>
	Iterator
	operator()(const interval_t& interval, Iterator iterator, solver_lowest) const
	{
		const auto result = _function(interval);

		if (boost::math::get<0>(result).lower() * boost::math::get<0>(result).upper() > 0)
			return iterator;

		if (boost::math::get<1>(result).lower() * boost::math::get<1>(result).upper() > 0)
		{
			if (boost::math::get<0>(_function(interval.lower())) * boost::math::get<0>(_function(interval.upper())) <= 0)
				*iterator++ = solve(interval);
			return iterator;
		}

		if (boost::numeric::width(interval) <= _eps)
			return iterator;

		const auto median = boost::numeric::median(interval);
		Iterator end = (*this)(interval_t(interval.lower(), median), iterator, solver_lowest());
		if (iterator != end)
			return end;
		end = (*this)(interval_t(median, interval.upper()), end, solver_lowest());

		return end;
	}

protected:
	interval_t::base_type
	solve(const interval_t& interval) const
	{
		return boost::math::tools::newton_raphson_iterate
		(
			_function,
			boost::numeric::median(interval),
			interval.lower(),
			interval.upper(),
			std::numeric_limits<interval_t::base_type>::digits
		);
	}

private:
	const Function& _function;
	interval_t::base_type _eps;
};

}
