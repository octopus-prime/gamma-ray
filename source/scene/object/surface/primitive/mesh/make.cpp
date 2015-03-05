/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/mesh/make.hpp>
#include <scene/object/surface/primitive/mesh/model.hpp>
#include <scene/object/surface/primitive/mesh/reader.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <tbb/parallel_for_each.h>
#include <boost/log/trivial.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<mesh::model>;

namespace mesh {

boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface mesh";

	// Read vertextes and faces
	const reader_t read = open(description->file);

	vertexes_t vertexes;
	faces_t faces;
	read(vertexes, faces);

	BOOST_LOG_TRIVIAL(trace) << "Vertexes: " << vertexes.size();
	BOOST_LOG_TRIVIAL(trace) << "Faces: " << faces.size();

	// Calculate normals
	normals_t normals(vertexes.size(), {{0, 0, 0}});

	tbb::parallel_for_each
	(
		faces.cbegin(), faces.cend(),
		[&vertexes, &faces, &normals](const face_t& face)
		{
			const vector3_t normal = (vertexes[face[1]] - vertexes[face[0]]) % (vertexes[face[2]] - vertexes[face[0]]);
			normals[face[0]] += normal;
			normals[face[1]] += normal;
			normals[face[2]] += normal;
		}
	);

	tbb::parallel_for_each
	(
		normals.begin(), normals.end(),
		[](vector3_t& normal)
		{
			normal = normalize(normal);
		}
	);

	// Build mesh
	mesh_t mesh = boost::make_shared<basic_mesh_t>(std::move(vertexes), std::move(faces), std::move(normals));

	// Build triangles and rtree
	triangles_t triangles;
	rtree_t rtree;

	geo::model::ring<vector3_t> ring;
	ring.resize(3);

	for (boost::uint32_t i = 0; i < mesh->size(); ++i)
	{
		triangles.emplace_back(mesh, i);

		for (std::size_t j = 0; j < 3; ++j)
			ring[j] = mesh->point(i, j);

		rtree.insert(value_t(geo::return_envelope<box_t>(ring), i));
	}

	const box_t box = transform
	(
		description->transformation,
		box_t// TODO: puke =>
		(
			{{
				geo::get<X>(rtree.bounds().min_corner()),
				geo::get<Y>(rtree.bounds().min_corner()),
				geo::get<Z>(rtree.bounds().min_corner())
			}},
			{{
				geo::get<X>(rtree.bounds().max_corner()),
				geo::get<Y>(rtree.bounds().max_corner()),
				geo::get<Z>(rtree.bounds().max_corner())
			}}
		)
	);

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>
		(
			description->transformation,
			std::move(triangles),
			std::move(rtree)
		),
		box
	);
}

}
}
}
}
}
}
