/*
 * vector.hpp
 *
 *  Created on: 05.01.2015
 *      Author: mike_gresens
 */

#pragma once

#include "container.hpp"
#include <boost/range/numeric.hpp>
#include <cmath>

namespace rt {

enum axis_t
{
  X, Y, Z, W
};

template <std::size_t N>
using basic_vector_t = basic_container_t<float, N>;

template <std::size_t N>
inline auto
operator*(const basic_vector_t<N>& v1, const basic_vector_t<N>& v2) noexcept
{
	return boost::inner_product(v1, v2, typename basic_vector_t<N>::value_type());
}

inline auto
operator%(const basic_vector_t<3>& v1, const basic_vector_t<3>& v2) noexcept
{
	return basic_vector_t<3>
	{{
		v1[Y] * v2[Z] - v1[Z] * v2[Y],
		v1[Z] * v2[X] - v1[X] * v2[Z],
		v1[X] * v2[Y] - v1[Y] * v2[X]
	}};
}

template <std::size_t N>
inline auto
operator&(const basic_vector_t<N>& v1, const basic_vector_t<N>& v2) noexcept
{
	basic_vector_t<N> r;
	boost::transform(v1, v2, r.begin(), [](const auto& e1, const auto& e2){return e1 * e2;});
	return r;
}

template <std::size_t N>
inline auto
operator|(const basic_vector_t<N>& v1, const basic_vector_t<N>& v2) noexcept
{
	basic_vector_t<N> r;
	boost::transform(v1, v2, r.begin(), [](const auto& e1, const auto& e2){return e1 / e2;});
	return r;
}

template <std::size_t N>
inline auto&
operator&=(basic_vector_t<N>& v1, const basic_vector_t<N>& v2) noexcept
{
	boost::transform(v1, v2, v1.begin(), [](const auto& e1, const auto& e2){return e1 * e2;});
	return v1;
}

template <std::size_t N>
inline auto&
operator|=(basic_vector_t<N>& v1, const basic_vector_t<N>& v2) noexcept
{
	boost::transform(v1, v2, v1.begin(), [](const auto& e1, const auto& e2){return e1 / e2;});
	return v1;
}

template <std::size_t N>
inline auto
length(const basic_vector_t<N>& vector) noexcept
{
	return std::sqrt(vector * vector);
}

template <std::size_t N>
inline auto
normalize(const basic_vector_t<N>& vector) noexcept
{
	return vector / length(vector);
}

typedef basic_vector_t<2> vector2_t;
typedef basic_vector_t<3> vector3_t;
typedef basic_vector_t<4> vector4_t;

}
