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

surface::instance_t
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(trace) << "Make mesh" << std::endl;

	// Read vertextes and faces
	const reader_t read = open(description->file);

	vertexes_t vertexes;
	faces_t faces;
	read(vertexes, faces);

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
//	mesh_t mesh(std::forward<vertexes_t>(vertexes), std::forward<faces_t>(faces), std::forward<normals_t>(normals));
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

	return primitive::make<model>
	(
		description->transformation,
		std::move(mesh),
		std::move(triangles),
		std::move(rtree)
	);
//	return primitive::make<model>
//	(
//		description->transformation,
//		std::forward<mesh_t>(mesh),
//		std::forward<triangles_t>(triangles),
//		std::forward<rtree_t>(rtree)
//	);
}

}
}
}
}
}
}
