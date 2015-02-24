/*
 * engine.cpp
 *
 *  Created on: 10.02.2015
 *      Author: mgresens
 */

#include <scene/description.hpp>
#include <parsing/parser.hpp>
#include <iostream>

#include <scene/object/surface/make.hpp>
#include <math/transformation.hpp>

using namespace rt;
using namespace scene::object::surface;

std::vector<base_t> preprocess(const std::string& file)
{
	const scene::description_t scene = parsing::parse(file);

	std::vector<base_t> surfaces;
	boost::transform
	(
		scene->objects,
		std::back_inserter(surfaces),
		[](const scene::object::description_t& description)
		{
			return make(description->surface);
		}
	);

	return std::move(surfaces);
}

int main(int argc, char** argv)
{
	try
	{
		const std::vector<base_t> surfaces = preprocess(argv[1]);
		std::cout << surfaces.size() << " surfaces" << std::endl;

		const ray_t ray { {{0,0,-2}}, {{0,0,+1}}, 0, 1e10 };
		hits_t hits(10);
		auto end = hits.begin();
		for (const auto& surface : surfaces)
			end = surface->hit(ray, end);
		std::cout << std::distance(hits.begin(), end) << " hits" << std::endl;
		std::cout << "min = " << std::min_element(hits.begin(), end)->point << std::endl;

		std::for_each
		(
			hits.begin(), end,
			[&surfaces](const hit_t& hit)
			{
					std::cout << hit.point << std::endl;
			}
		);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "error: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}
}
