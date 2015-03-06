/*
 * matrix.hpp
 *
 *  Created on: 10.01.2015
 *      Author: mike_gresens
 */

#pragma once

#include "vector.hpp"

namespace rt {

template <std::size_t M, std::size_t N>
using basic_matrix_t = basic_container_t<basic_vector_t<N>, M>;

template <std::size_t N>
inline auto
identity() noexcept
{
	typedef typename basic_matrix_t<N, N>::value_type::value_type E;

	basic_matrix_t<N, N> r;
	for (std::size_t i = 0; i < N; ++i)
	{
		r[i].fill(E(0));
		r[i][i] = E(1);
	}
	return r;
}

template <std::size_t N>
inline auto
transpose(const basic_matrix_t<N, N>& m) noexcept
{
	basic_matrix_t<N, N> r;
	for (std::size_t i = 0; i < N; ++i)
		for (std::size_t j = 0; j < N; ++j)
			r[i][j] = m[j][i];
	return r;
}

template <std::size_t N>
inline auto
invert(basic_matrix_t<N, N> m)
{
	basic_matrix_t<N, N> r = identity<N>();
/*
	for (std::size_t i = N - 1; i > 0; --i)
	{
		if (m[i-1][0] < m[i][0])
		{
			std::swap(m[i-1], m[i]);
			std::swap(r[i-1], r[i]);
		}
	}
*/
	for (std::size_t i = 0; i < N; ++i)
	{
		for (std::size_t j = 0; j < N; ++j)
		{
			if (i == j)
				continue;

			const float d = m[j][i] / m[i][i];
			m[j] -= m[i] * d;
			r[j] -= r[i] * d;
		}
	}

	for (std::size_t i = 0; i < N; ++i)
	{
		const float d = m[i][i];
		m[i] /= d;
		r[i] /= d;
	}

	return r;
}

template <std::size_t M, std::size_t N, std::size_t O>
inline auto
operator*(const basic_matrix_t<M, N>& m1, const basic_matrix_t<N, O>& m2) noexcept
{
	typedef typename basic_matrix_t<N, N>::value_type::value_type E;

	basic_matrix_t<M, O> r;
	for (std::size_t i = 0; i < M; ++i)
		for (std::size_t j = 0; j < O; ++j)
		{
			float s = E(0);
			for (std::size_t k = 0; k < N; ++k)
				s += m1[i][k] * m2[k][j];
			r[i][j] = s;
		}
	return r;
}

template <std::size_t N>
inline auto&
operator*=(basic_matrix_t<N, N>& m1, const basic_matrix_t<N, N>& m2) noexcept
{
	return m1 = m1 * m2;
}

template <std::size_t M, std::size_t N>
inline auto
operator*(const basic_matrix_t<M, N>& m, const basic_vector_t<N>& v) noexcept
{
	basic_vector_t<M> r;
	boost::transform(m, r.begin(), [&v](const basic_vector_t<N>& e){return e * v;});
	return r;
}

template <std::size_t M, std::size_t N>
inline auto
operator*(const basic_vector_t<N>& v, const basic_matrix_t<M, N>& m) noexcept
{
	typedef typename basic_matrix_t<N, N>::value_type::value_type E;

	basic_vector_t<N> r;
    for (std::size_t j = 0; j < N; ++j)
    {
		float s = E(0);
        for (std::size_t k = 0; k < M; ++k)
            s += v[k] * m[k][j];
        r[j] = s;
    }
    return r;
}

typedef basic_matrix_t<2,2> matrix22_t;
typedef basic_matrix_t<2,3> matrix23_t;
typedef basic_matrix_t<2,4> matrix24_t;
typedef basic_matrix_t<3,2> matrix32_t;
typedef basic_matrix_t<3,3> matrix33_t;
typedef basic_matrix_t<3,4> matrix34_t;
typedef basic_matrix_t<4,2> matrix42_t;
typedef basic_matrix_t<4,3> matrix43_t;
typedef basic_matrix_t<4,4> matrix44_t;

}
