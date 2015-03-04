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
#include <boost/container/static_vector.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>

namespace rt {
namespace scene {

typedef std::pair<box_t, boost::uint32_t> value_t;
typedef geo::index::rtree<value_t, geo::index::quadratic<8>> rtree_t;

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
//		constexpr boost::uint32_t missed = -1;

		rendering::hits_t::iterator foo = hits;
		const auto f = [this, &ray, &hits, &foo](const value_t& value)
		{
			const object::instance_t& object = _objects[value.second];
			foo = object.hit(ray, hits);
			return foo != hits;
		};

		static constexpr std::size_t MAX = 1;
		typedef boost::container::static_vector<value_t, MAX> values_t;

		const segment_t segment = ray;

		values_t values;
		_rtree.query
		(
			geo::index::intersects(segment) &&
			geo::index::satisfies(f) &&
			geo::index::nearest(segment.first, MAX),
			std::back_inserter(values)
		);

		return foo;
/*
		return boost::accumulate
		(
			values,
			hits,
			[this, &ray](const rendering::hits_t::iterator hit, const value_t& value)
			{
//				std::cout << "bounding hit" << std::endl;
				const object::instance_t& object = _objects[value.second];
				return object.hit(ray, hit);
			}
		);
*/
	}

	bool
	any(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		constexpr boost::uint32_t missed = -1;

		const auto f = [this, &ray, &hits](const value_t& value)
		{
			const object::instance_t& object = _objects[value.second];
			return object.hit(ray, hits) != hits;
		};

		const segment_t segment = ray;

		value_t v(box_t(), missed);
		_rtree.query
		(
			geo::index::intersects(segment) &&
			geo::index::satisfies(f) &&
			geo::index::nearest(segment.first, 1),
			&v
		);

		return v.second != missed;
	}

private:
	camera::instance_t _camera;
	light::instances_t _lights;
	object::instances_t _objects;
	rtree_t _rtree;
};

}
}
