/*
 * ray.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <math/homogeneous.hpp>
#include <geo/segment.hpp>

namespace rt {
namespace rendering {

class ray_t
{
	static constexpr float epsilon = 1e-3;

public:
	ray_t(const vector3_t& origin, const vector3_t& direction, const float min = 0.0f, const float max = std::numeric_limits<float>::max() * 0.1f) noexcept
//	ray_t(const vector3_t& origin, const vector3_t& direction, const float min = 0.0f, const float max = 1e5f) noexcept
	:
		_origin(origin),
		_direction(direction),
		_min(min + epsilon),
		_max(max - epsilon)
	{
	}

	const vector3_t& origin() const noexcept
	{
		return _origin;
	}

	const vector3_t& direction() const noexcept
	{
		return _direction;
	}

	float min() const noexcept
	{
		return _min;
	}

	float max() const noexcept
	{
		return _max;
	}

	vector3_t operator()(const float distance) const noexcept
	{
		return _origin + _direction * distance;
	}

	bool operator[](const float distance) const noexcept
	{
		return _min < distance && distance < _max;
	}

	ray_t operator()(const matrix44_t& transformation) const noexcept
	{
		return
		{
			point::operator*(transformation, _origin),
			direction::operator*(transformation, _direction),
			_min,
			_max,
		};
	}

	operator segment_t() const noexcept
	{
		return segment_t((*this)(_min), (*this)(_max));
	}

private:
	vector3_t _origin;
	vector3_t _direction;
	float _min;
	float _max;
};

}
}
/*
namespace boost {
namespace geometry {
namespace traits {

//template <typename Point>
template <>
struct tag<rt::rendering::ray_t>
{
    typedef segment_tag type;
};

//template <typename Point>
template <>
struct point_type<rt::rendering::ray_t>
{
    typedef rt::vector3_t type;
};

//template <typename Point, std::size_t Dimension>
template <std::size_t Dimension>
struct indexed_access<rt::rendering::ray_t, 0, Dimension>
{
    typedef rt::rendering::ray_t segment_type;
    typedef typename geometry::cs::cartesian coordinate_type;

    static inline coordinate_type get(segment_type const& s)
    {
        return geometry::get<Dimension>(s(s.min()));
    }

    static inline void set(segment_type& s, coordinate_type const& value)
    {
//        geometry::set<Dimension>(s.first, value);
    }
};

//template <typename Point, std::size_t Dimension>
template <std::size_t Dimension>
struct indexed_access<rt::rendering::ray_t, 1, Dimension>
{
    typedef rt::rendering::ray_t segment_type;
    typedef typename geometry::cs::cartesian coordinate_type;

    static inline coordinate_type get(segment_type const& s)
    {
        return geometry::get<Dimension>(s(s.max()));
    }

    static inline void set(segment_type& s, coordinate_type const& value)
    {
//        geometry::set<Dimension>(s.first, value);
    }
};

}
}
}
*/
