/*
 * instance.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/camera/instance.hpp>
#include <scene/light/instance.hpp>
#include <scene/object/instance.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace rt {
namespace scene {

typedef std::pair<box_t, boost::uint32_t> value_t;
typedef geo::index::rtree<value_t, geo::index::rstar<8>> rtree_t;

class instance_t
{
public:
	instance_t(camera::instance_t&& camera, light::instances_t&& lights, object::instances_t&& objects, rtree_t&& rtree)
	:
		_camera(camera),
		_lights(lights),
		_objects(objects),
		_rtree(rtree)
	{
	}

	const camera::instance_t&
	camera() const noexcept
	{
		return _camera;
	}

	const light::instances_t&
	lights() const noexcept
	{
		return _lights;
	}

	const object::instances_t&
	objects() const noexcept
	{
		return _objects;
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const segment_t segment = ray;

		rendering::hits_t::iterator end = hits;
		const auto test = [this, &ray, &hits, &end, &segment](const value_t& value)
		{
			if (geo::intersects(segment, value.first))
			{
				const object::instance_t& object = _objects[value.second];
				end = object.hit(ray, end);
			}
			return false;
		};

		value_t value;
		_rtree.query(geo::index::satisfies(test), &value);

		if (end == hits)
			return hits;

		*hits = *std::min_element(hits, end);

		return hits + 1;
	}

private:
	camera::instance_t _camera;
	light::instances_t _lights;
	object::instances_t _objects;
	rtree_t _rtree;
};

}
}
