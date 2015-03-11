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

template <typename Function>
class solver_t
{
public:
	template<typename... Args>
	constexpr
	solver_t(Args&&... args)
	:
		_function(std::forward<Args>(args)...)
	{
	}

	template <typename Iterator>
	Iterator
	operator()(const interval_t& interval, Iterator iterator) const
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

		if (std::abs(interval.lower() - interval.upper()) <= std::numeric_limits<interval_t::base_type>::epsilon())
			return iterator;

		const auto middle = interval_t::base_type(0.5) * (interval.lower() + interval.upper());
		iterator = (*this)(interval_t(interval.lower(), middle), iterator);
		iterator = (*this)(interval_t(middle, interval.upper()), iterator);

		return iterator;
	}

protected:
	interval_t::base_type
	solve(const interval_t& interval) const
	{
		return boost::math::tools::newton_raphson_iterate
		(
			_function,
			interval_t::base_type(0.5) * (interval.lower() + interval.upper()),
			interval.lower(),
			interval.upper(),
			std::numeric_limits<interval_t::base_type>::digits
		);
	}

private:
	Function _function;
};

}
