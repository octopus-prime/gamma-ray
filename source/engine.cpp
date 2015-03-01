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
#include <rendering/renderer.hpp>
#include <rendering/image.hpp>
#include <configuration.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include <scene/object/texture/noise/make.hpp>
#include <boost/make_shared.hpp>

using namespace rt;

void test_noise()
{
	using namespace scene::object::texture::noise;

	const double desc1(50.0);
	const auto desc2 = boost::make_shared<generator::cylinders::basic_description_t>();
	desc2->frequency = 16.0;
	const auto desc3 = boost::make_shared<combiner::basic_description_t<combiner::add_tag>>(desc1, desc2);
	const auto desc4 = boost::make_shared<combiner::basic_description_t<combiner::mul_tag>>(desc1, desc3);

	const auto noise = make(desc4);

	std::cout << noise({{1,2,3}}) << std::endl;
}

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
//		test_noise();
//		exit(0);

		const configuration config(argc, argv);
		const auto input = config.get_required<std::string>(configuration::INPUT);
		const auto output = config.get_required<std::string>(configuration::OUTPUT);
		const auto width = config.get_required<std::size_t>(configuration::WIDTH);
		const auto height = config.get_required<std::size_t>(configuration::HEIGHT);
		const auto depth = config.get_required<std::size_t>(configuration::DEPTH);
		const auto aa = config.get_required<std::size_t>(configuration::AA);

		const scene::instance_t scene = preprocess(input);
		std::cout << scene.objects().size() << " objects" << std::endl;

		std::cout << "Lights       = " << scene.lights().size() << std::endl;
		std::cout << "Objects      = " << scene.objects().size() << std::endl;
		std::cout << "Antialiasing = " << static_cast<std::size_t>(aa) << std::endl;
		std::cout << "Depth        = " << depth << std::endl;
		std::cout << "Resolution   = " << width << 'x' << height << std::endl;

//		const auto output_path = boost::filesystem::path("pictures") / output;//"picture.png";
		const auto output_path = boost::filesystem::path(output);//"picture.png";

		std::cout << "Output       = " << output << std::endl;

		boost::filesystem::create_directories(output_path.parent_path());
		const rendering::image_writer_t write = rendering::make_writer(output_path.extension().string());

		const rendering::renderer_t render(scene, static_cast<rendering::AA>(aa), depth);
		rendering::image_t image(width, height);

		render(boost::gil::view(image));
		write(boost::gil::view(image), output_path.string());

		return EXIT_SUCCESS;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "error: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}
}
