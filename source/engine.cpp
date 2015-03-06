/*
 * engine.cpp
 *
 *  Created on: 10.02.2015
 *      Author: mgresens
 */

#include <configuration.hpp>
#include <logging.hpp>
#include <parsing/parser.hpp>
#include <scene/description.hpp>
#include <scene/instance.hpp>
#include <scene/make.hpp>
#include <rendering/renderer.hpp>
#include <rendering/image.hpp>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

/*
//#include <scene/object/texture/noise/make.hpp>
//#include <boost/make_shared.hpp>

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
*/

static rt::configuration_t
configure(const int argc, const char* const argv[])
{
	const rt::configuration_t configuration = rt::configure(argc, argv);
	rt::logging(configuration.level.at(0));

	BOOST_LOG_NAMED_SCOPE("Configuration");
	BOOST_LOG_TRIVIAL(info) << "Configuration start";
	BOOST_LOG_TRIVIAL(info) << "Input " << '"' << configuration.input << '"';
	BOOST_LOG_TRIVIAL(info) << "Output " << '"' << configuration.output << '"';
	BOOST_LOG_TRIVIAL(info) << "Resolution " << configuration.width << 'x' << configuration.height;
	BOOST_LOG_TRIVIAL(info) << "Recursion " << configuration.depth;
	BOOST_LOG_TRIVIAL(info) << "Antialiasing " << configuration.aa;
	BOOST_LOG_TRIVIAL(info) << "Configuration done" << std::endl;

	return std::move(configuration);
}

static rt::scene::instance_t
preprocess(const std::string& file)
{
	BOOST_LOG_NAMED_SCOPE("Preprocessing");
	BOOST_LOG_TRIVIAL(info) << "Preprocessing start";

	const rt::scene::description_t description = rt::parsing::parse(file);
	const rt::scene::instance_t instance = rt::scene::make(description);

	BOOST_LOG_TRIVIAL(info) << "Created " << instance.lights().size() << " lights";
	BOOST_LOG_TRIVIAL(info) << "Created " << instance.objects().size() << " objects";
	BOOST_LOG_TRIVIAL(info) << "Preprocessing done" << std::endl;

	return std::move(instance);
}

static rt::rendering::image_t
render(const rt::configuration_t& configuration, const rt::scene::instance_t& scene)
{
	BOOST_LOG_NAMED_SCOPE("Rendering");
	BOOST_LOG_TRIVIAL(info) << "Rendering start";

	const rt::rendering::renderer_t render(scene, static_cast<rt::rendering::AA>(configuration.aa), configuration.depth);
	rt::rendering::image_t image(configuration.width, configuration.height);

	render(boost::gil::view(image));

	BOOST_LOG_TRIVIAL(info) << "Rendering done" << std::endl;

	return std::move(image);
}

static void
postprocess(const rt::rendering::image_writer_t& write, const rt::rendering::image_t& image)
{
	BOOST_LOG_NAMED_SCOPE("Postprocessing");
	BOOST_LOG_TRIVIAL(info) << "Postprocessing start";

	write(boost::gil::const_view(image));

	BOOST_LOG_TRIVIAL(info) << "Postprocessing done" << std::endl;
}

int
main(int argc, char** argv)
{
	std::cout << std::endl << "γ-ray version 0.1" << std::endl << std::endl;

	try
	{
		const rt::configuration_t configuration = configure(argc, argv);
		const rt::rendering::image_writer_t write = rt::rendering::make_writer(configuration.output);
		const rt::scene::instance_t scene = preprocess(configuration.input);
		const rt::rendering::image_t image = render(configuration, scene);
		postprocess(write, image);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "error: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}
}
