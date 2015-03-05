/*
 * container.hpp
 *
 *  Created on: 20.01.2015
 *      Author: mike_gresens
 */

#pragma once

#include <boost/array.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <iostream>

namespace rt {

template <typename E, std::size_t N>
using basic_container_t = boost::array<E, N>;

template <typename E, std::size_t N>
inline constexpr const auto&
operator+(const basic_container_t<E, N>& c) noexcept
{
	return c;
}

template <typename E, std::size_t N>
inline auto
operator-(const basic_container_t<E, N>& c) noexcept
{
	basic_container_t<E, N> r;
	boost::transform(c, r.begin(), [](const E& e){return -e;});
	return r;
}

template <typename E, std::size_t N>
inline auto
operator+(const basic_container_t<E, N>& c1, const basic_container_t<E, N>& c2) noexcept
{
	basic_container_t<E, N> r;
	boost::transform(c1, c2, r.begin(), [](const E& e1, const E& e2){return e1 + e2;});
	return r;
}

template <typename E, std::size_t N>
inline auto
operator-(const basic_container_t<E, N>& c1, const basic_container_t<E, N>& c2) noexcept
{
	basic_container_t<E, N> r;
	boost::transform(c1, c2, r.begin(), [](const E& e1, const E& e2){return e1 - e2;});
	return r;
}

template <typename E, typename F, std::size_t N>
inline auto
operator*(const basic_container_t<E, N>& c, const F& f) noexcept
{
	basic_container_t<E, N> r;
	boost::transform(c, r.begin(), [&f](const E& e){return e * f;});
	return r;
}

template <typename E, typename F, std::size_t N>
inline auto
operator*(const F& f, const basic_container_t<E, N>& c) noexcept
{
	basic_container_t<E, N> r;
	boost::transform(c, r.begin(), [&f](const E& e){return e * f;});
	return r;
}

template <typename E, typename F, std::size_t N>
inline auto
operator/(const basic_container_t<E, N>& c, const F& f) noexcept
{
	basic_container_t<E, N> r;
	boost::transform(c, r.begin(), [&f](const E& e){return e / f;});
	return r;
}

template <typename E, std::size_t N>
inline auto&
operator+=(basic_container_t<E, N>& c1, const basic_container_t<E, N>& c2) noexcept
{
	boost::transform(c1, c2, c1.begin(), [](const E& e1, const E& e2){return e1 + e2;});
	return c1;
}

template <typename E, std::size_t N>
inline auto&
operator-=(basic_container_t<E, N>& c1, const basic_container_t<E, N>& c2) noexcept
{
	boost::transform(c1, c2, c1.begin(), [](const E& e1, const E& e2){return e1 - e2;});
	return c1;
}

template <typename E, typename F, std::size_t N>
inline auto&
operator*=(basic_container_t<E, N>& c, const F& f) noexcept
{
	boost::for_each(c, [&f](E& e){ e *= f;});
	return c;
}

template <typename E, typename F, std::size_t N>
inline auto&
operator/=(basic_container_t<E, N>& c, const F& f) noexcept
{
	boost::for_each(c, [&f](E& e){ e /= f;});
	return c;
}

}

namespace std {

template <typename T, std::size_t N>
inline ostream&
operator<<(ostream& s, const rt::basic_container_t<T, N>& c)
{
	s << '<';
	for (auto e = c.cbegin(); e != c.cend(); ++e)
	{
		if (e != c.cbegin())
			s << ", ";
		s << std::showpos /*<< std::showpoint*/ << *e;
	}
	s << '>';
	return s;
}

}
