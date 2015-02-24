/*
 * engine.cpp
 *
 *  Created on: 10.02.2015
 *      Author: mgresens
 */

#include <scene/description.hpp>
#include <scene/instance.hpp>
#include <scene/make.hpp>
#include <parsing/parser.hpp>
#include <iostream>

using namespace rt;

scene::instance_t
preprocess(const std::string& file)
{
	const scene::description_t description = parsing::parse(file);
	const scene::instance_t instance = scene::make(description);
	return std::move(instance);
}

int main(int argc, char** argv)
{
	try
	{
		const scene::instance_t scene = preprocess(argv[1]);
		std::cout << scene.objects().size() << " objects" << std::endl;

		const rendering::ray_t ray { {{0,0,-2}}, {{0,0,+1}}, 0, 1e10 };
		rendering::hits_t hits(10);
		auto end = hits.begin();
		for (const auto& object : scene.objects())
			end = object.hit(ray, end);
		std::cout << std::distance(hits.begin(), end) << " hits" << std::endl;
		std::cout << "min = " << std::min_element(hits.begin(), end)->point << std::endl;

		std::for_each
		(
			hits.begin(), end,
			[](const rendering::hit_t& hit)
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
