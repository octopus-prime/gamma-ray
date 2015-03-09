/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/mesh/model_fwd.hpp>
#include <geo/segment.hpp>
#include <geo/box.hpp>
#include <boost/geometry/index/rtree.hpp>

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

					hits->distance = t;
					hits->normal = n;

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
typedef std::pair<box_t, boost::uint32_t> value_t;
typedef geo::index::rtree<value_t, geo::index::quadratic<8>> rtree_t; // TODO: parse parameter

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
		rendering::hits_t::iterator end = hits;
		const auto test = [this, &ray, &end](const value_t& value)
		{
			const triangle_t& triangle = _triangles[value.second];
			end = triangle.hit(ray, end);
			return false;
		};

		const segment_t segment = ray;
		_rtree.query(geo::index::intersects(segment) && geo::index::satisfies(test), (value_t*) nullptr);

		if (end == hits)
			return hits;

		*hits = *std::min_element(hits, end); // TODO: deliver max element too ?!

		return hits + 1;
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
