/*
 * spline.hpp
 *
 *  Created on: 16.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <math/polynomial.hpp>
#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/tuple/tuple.hpp>

namespace rt {

typedef boost::tuple<float, float, polynomial3_t> spline_segment_t;
typedef std::vector<spline_segment_t> spline_t;

inline float
evaluate(const spline_t& spline, const float x)
{
	const spline_t::const_iterator segment = boost::lower_bound
	(
		spline, x,
		[](const spline_segment_t& segment, const float x)
		{
			return segment.get<1>() < x;
		}
	);
	if (segment == spline.end())
		return evaluate(spline.back().get<2>(), x - spline.back().get<0>());
	return evaluate(segment->get<2>(), x - segment->get<0>());
}

template <typename Iterator>
inline spline_t
make_spline(const Iterator begin, const Iterator end)
{
	const int n = std::distance(begin, end) - 1;

	std::vector<float> a;
	std::transform
	(
		begin, end,
		std::back_inserter(a),
		[](const vector2_t& point)
		{
			return point[Y];
		}
	);

	std::vector<float> b(n);
	std::vector<float> d(n);

	std::vector<float> h;
	std::transform
	(
		begin + 1, end,
		begin,
		std::back_inserter(h),
		[](const vector2_t& point1, const vector2_t& point2)
		{
			return point1[X] - point2[X];
		}
	);

	std::vector<float> alpha;
	for(int i = 0; i < n; ++i)
		alpha.push_back( 3*(a[i+1]-a[i])/h[i] - 3*(a[i]-a[i-1])/h[i-1]  );

	std::vector<float> c(n+1);
	std::vector<float> l(n+1);
	std::vector<float> mu(n+1);
	std::vector<float> z(n+1);
	l[0] = 1;
	mu[0] = 0;
	z[0] = 0;

	for(int i = 1; i < n; ++i)
	{
		l[i] = 2 *(begin[i+1][X] - begin[i-1][X])-h[i-1]*mu[i-1];
		mu[i] = h[i]/l[i];
		z[i] = (alpha[i]-h[i-1]*z[i-1])/l[i];
	}

	l[n] = 1;
	z[n] = 0;
	c[n] = 0;

	for(int j = n-1; j >= 0; --j)
	{
		c[j] = z [j] - mu[j] * c[j+1];
		b[j] = (a[j+1]-a[j])/h[j]-h[j]*(c[j+1]+2*c[j])/3;
		d[j] = (c[j+1]-c[j])/3/h[j];
	}

	spline_t spline;
	for(int i = 0; i < n; ++i)
		spline.emplace_back
		(
			begin[i][X],
			begin[i+1][X],
			polynomial3_t {{a[i], b[i], c[i], d[i]}}
		);

	return std::move(spline);

}

}
