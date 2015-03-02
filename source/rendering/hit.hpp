/*
 * hit.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <vector>
#include <boost/iterator/iterator_adaptor.hpp>

namespace rt {

namespace scene {
namespace object {

class instance_t;

}
}

namespace rendering {

struct hit_t
{
	double distance;
	vector3_t point;
	vector3_t normal;
	const scene::object::instance_t* object;

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

}
}
