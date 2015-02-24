/*
 * base.hpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <math/homogeneous.hpp>
#include <vector>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {

struct ray_t
{
	const vector3_t origin;
	const vector3_t direction;
	const float min;
	const float max;

	vector3_t operator()(const float distance) const noexcept
	{
		return origin + direction * distance;
	}

	bool operator[](const float distance) const noexcept
	{
		return min < distance && distance < max;
	}

	ray_t operator()(const matrix44_t& transformation) const noexcept
	{
		return
		{
			point::operator*(transformation, origin),
			direction::operator*(transformation, direction),
			min,
			max,
		};
	}
};

struct hit_t
{
	double distance;
	vector3_t point;
	vector3_t normal;
//	const base_object_t* object;

	bool operator<(const hit_t& hit) const
	{
		return distance < hit.distance;
	}
};

typedef std::vector<hit_t> hits_t;

class distance_iterator
:
	public boost::iterator_adaptor<distance_iterator, hits_t::iterator, double, boost::random_access_traversal_tag>
{
    friend class boost::iterator_core_access;

public:
    explicit distance_iterator(const hits_t::iterator iterator)
      : distance_iterator::iterator_adaptor_(iterator) {}

protected:
    reference dereference() const
    {
    	return base()->distance;
    }
};

class normal_iterator
:
	public boost::iterator_adaptor<normal_iterator, hits_t::iterator, vector3_t, boost::random_access_traversal_tag>
{
    friend class boost::iterator_core_access;

public:
    explicit normal_iterator(const hits_t::iterator iterator)
      : normal_iterator::iterator_adaptor_(iterator) {}

protected:
    reference dereference() const
    {
    	return base()->normal;
    }
};

class base
{
protected:
	virtual ~base() noexcept = default;

public:
	virtual hits_t::iterator hit(const ray_t& ray, const hits_t::iterator hits) const = 0;
	virtual bool inside(const vector3_t& point) const = 0;
};

typedef boost::shared_ptr<base> base_t;

}
}
}
}
