/*
 * homogeneous.hpp
 *
 *  Created on: 12.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <math/matrix.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/numeric.hpp>

namespace rt {
namespace point {

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N>& v1, const basic_vector_t<N-1>& v2)
{
	static_assert(N > 1, "N > 1 failed");
	return boost::inner_product(v2, v1, v1.back());
}

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N-1>& v1, const basic_vector_t<N>& v2)
{
	static_assert(N > 1, "N > 1 failed");
	return boost::inner_product(v1, v2, v2.back());
}

template <std::size_t N>
inline auto
operator*(const basic_matrix_t<N,N>& matrix, const basic_vector_t<N-1>& vector)
{
	static_assert(N > 1, "N > 1 failed");
	basic_vector_t<N-1> result;
	std::transform
	(
		matrix.cbegin(), matrix.cend() - 1,
		result.begin(),
		[&vector](const basic_vector_t<N>& row)
		{
			return vector * row;
		}
	);
	return result;
}

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N-1>& vector, const basic_matrix_t<N,N>& matrix)
{
	static_assert(N > 1, "N > 1 failed");
	basic_vector_t<N> result;
    for (std::size_t j = 0; j < N; ++j)
    {
        float s = matrix[N - 1][j];
        for (std::size_t k = 0; k < N - 1; ++k)
            s += vector[k] * matrix[k][j];
        result[j] = s;
    }
    return result;
}

}

namespace direction {

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N>& v1, const basic_vector_t<N-1>& v2)
{
	static_assert(N > 1, "N > 1 failed");
	return boost::inner_product(v2, v1, 0.0f);
}

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N-1>& v1, const basic_vector_t<N>& v2)
{
	static_assert(N > 1, "N > 1 failed");
	return boost::inner_product(v1, v2, 0.0f);
}

template <std::size_t N>
inline auto
operator*(const basic_matrix_t<N,N>& matrix, const basic_vector_t<N-1>& vector)
{
	static_assert(N > 1, "N > 1 failed");
	basic_vector_t<N-1> result;
	std::transform
	(
		matrix.cbegin(), matrix.cend() - 1,
		result.begin(),
		[&vector](const basic_vector_t<N>& row)
		{
			return vector * row;
		}
	);
	return result;
}

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N-1>& vector, const basic_matrix_t<N,N>& matrix)
{
	static_assert(N > 1, "N > 1 failed");
	basic_vector_t<N> result;
    for (std::size_t j = 0; j < N; ++j)
    {
        float s = 0.0f;
        for (std::size_t k = 0; k < N - 1; ++k)
            s += vector[k] * matrix[k][j];
        result[j] = s;
    }
    return result;
}

}

}
