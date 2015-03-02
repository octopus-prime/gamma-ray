/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/mesh/model_fwd.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/adapted/boost_array.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/container/static_vector.hpp>

namespace geo = boost::geometry;

BOOST_GEOMETRY_REGISTER_BOOST_ARRAY_CS(geo::cs::cartesian);

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace mesh {

class basic_mesh_t
{
public:
	basic_mesh_t(vertexes_t&& vertexes, faces_t&& faces, normals_t&& normals)
	:
		_vertexes(std::forward<vertexes_t>(vertexes)),
		_faces(std::forward<faces_t>(faces)),
		_normals(std::forward<normals_t>(normals))
	{
	}

	std::size_t
	size() const
	{
		return _faces.size();
	}

	const vector3_t&
	point(const std::size_t i, const std::size_t j) const
	{
		return _vertexes[_faces[i][j]];
	}

	const vector3_t&
	normal(const std::size_t i, const std::size_t j) const
	{
		return _normals[_faces[i][j]];
	}

private:
	vertexes_t _vertexes;
	faces_t _faces;
	normals_t _normals;
};

typedef boost::shared_ptr<basic_mesh_t> mesh_t;

class triangle_t
{
	typedef boost::array<vector3_t, 2> edge_t;	// Plücker's U,V

public:
	triangle_t(const mesh_t& mesh, const boost::uint32_t index)
	:
		_mesh(mesh),
		_index(index),
		_normal(normalize((point(1) - point(0)) % (point(2) - point(0)))),
		_distance(_normal * point(0))
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const edge_t edge {{ray.direction(), ray.direction() % ray.origin()}};

		const bool side0 = test<1,0>(edge);
		const bool side1 = test<2,1>(edge);

		if (side0 == side1)
		{
			const bool side2 = test<0,2>(edge);
			if (side0 == side2)
			{
				const float t = (_distance - _normal * ray.origin()) / (_normal * ray.direction());
				if (ray[t])
				{
					const vector3_t p = ray(t);

					const float a = area(point(0), point(1), point(2));
					const float a1 = area(p, point(0), point(2)) / a;
					const float a2 = area(p, point(0), point(1)) / a;
					const float a0 = 1.f - a1 - a2;

					const vector3_t n = normal(0) * a0 + normal(1) * a1 + normal(2) * a2;

					*hits = rendering::hit_t {t, p, n, nullptr};
					return hits + 1;
				}
			}
		}

		return hits;
	}

protected:
	template <std::size_t F, std::size_t S>
	bool
	test(const edge_t& edge) const
	{
		const edge_t temp {{point(F) - point(S), point(F) % point(S)}};
		return edge[0] * temp[1] + edge[1] * temp[0] >= 0.0;
	}

	static float
	area(const vector3_t& p0, const vector3_t& p1, const vector3_t& p2)
	{
		return length((p1 - p0) % (p2 - p0));
	}

	const vector3_t&
	point(const std::size_t index) const
	{
		return _mesh->point(_index, index);
	}

	const vector3_t&
	normal(const std::size_t index) const
	{
		return _mesh->normal(_index, index);
	}

private:
	const mesh_t _mesh;
	const boost::uint32_t _index;
	vector3_t _normal;
	float _distance;
};

typedef std::vector<triangle_t> triangles_t;

static constexpr std::size_t MAX = 32; // todo: parse

typedef geo::model::box<vector3_t> box_t;
typedef std::pair<box_t, boost::uint32_t> value_t;
typedef boost::container::static_vector<value_t, MAX> values_t;
typedef geo::index::rtree<value_t, geo::index::quadratic<8>> rtree_t;

class model
{
public:
	model(triangles_t&& triangles, rtree_t&& rtree)
	:
		_triangles(std::forward<triangles_t>(triangles)),
		_rtree(std::forward<rtree_t>(rtree))
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		typedef geo::model::segment<vector3_t> segment_t;

		const segment_t segment(ray(ray.min()), ray(ray.max()));

		values_t values;
		_rtree.query
		(
			geo::index::intersects(segment) &&
			geo::index::nearest(segment.first, MAX),
			std::back_inserter(values)
		);

		return boost::accumulate
		(
			values,
			hits,
			[this, &ray](const rendering::hits_t::iterator hit, const value_t& value)
			{
				const triangle_t& triangle = _triangles[value.second];
				return triangle.hit(ray, hit);
			}
		);
	}

	bool
	inside(const vector3_t& point) const
	{
		return false;	// todo
	}

private:
	triangles_t _triangles;
	rtree_t _rtree;
};

}
}
}
}
}
}
