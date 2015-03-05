/*
 * configuration.cpp
 *
 *  Created on: 25.02.2015
 *      Author: mike_gresens
 */

#include "configuration.hpp"
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/optional.hpp>
#include <fstream>
#include <iostream>

namespace po = boost::program_options;

namespace rt {

static const std::string HELP("help");
static const std::string CONFIG("config");
static const std::string INPUT("input");
static const std::string OUTPUT("output");
static const std::string WIDTH("width");
static const std::string HEIGHT("height");
static const std::string DEPTH("depth");
static const std::string AA("aa");
static const std::string LEVEL("level");

template <typename T>
static T
get_required(const po::variables_map& vm, const std::string& option)
{
	const auto value = vm.find(option);
	if (value == vm.end())
		throw std::runtime_error("Missing option: " + option);
	return value->second.as<T>();
}

template <typename T>
static boost::optional<T>
get_optional(const po::variables_map& vm, const std::string& option)
{
	const auto value = vm.find(option);
	if (value == vm.end())
		return boost::none;
	return value->second.as<T>();
}

configuration_t
configure(const int argc, const char* const argv[])
{
	po::options_description description;

	po::options_description actions("Actions");
	actions.add_options()
		((HELP + ",H").c_str(),	"help")
	;

	po::options_description options("Options");
	options.add_options()
		((CONFIG + ",c").c_str(),	po::value<std::string>(),	"config file")
		((INPUT  + ",i").c_str(),	po::value<std::string>(),	"input file")
		((OUTPUT + ",o").c_str(),	po::value<std::string>(),	"output file")
		((WIDTH  + ",w").c_str(),	po::value<std::size_t>(),	"resolution width")
		((HEIGHT + ",h").c_str(),	po::value<std::size_t>(),	"resolution height")
		((DEPTH  + ",d").c_str(),	po::value<std::size_t>(),	"rendering depth")
		((AA     + ",a").c_str(),	po::value<std::size_t>(),	"anti aliasing")
		((LEVEL  + ",l").c_str(),	po::value<std::string>(),	"log level")
	;

	description.add(actions);
	description.add(options);

	po::variables_map vm;

	po::store(po::parse_command_line(argc, argv, description), vm);
	po::notify(vm);

	if (argc == 1 || vm.empty() || vm.count(HELP))
	{
		std::cout << std::endl;
		std::cout << "Usage: " << argv[0] << " <action> <options>" << std::endl;
		std::cout << description << std::endl;
		exit(vm.count(HELP) == 1);
	}

	const auto config = get_optional<std::string>(vm, CONFIG);
	if (config)
	{
		std::ifstream file(*config);
		if (file.fail())
			throw std::runtime_error(*config + " not found.");
		po::store(po::parse_config_file(file, description), vm);
		file.close();
		po::notify(vm);
	}

	return configuration_t
	{
		get_required<std::string>(vm, INPUT),
		get_required<std::string>(vm, OUTPUT),
		get_required<std::size_t>(vm, WIDTH),
		get_required<std::size_t>(vm, HEIGHT),
		get_required<std::size_t>(vm, DEPTH),
		get_required<std::size_t>(vm, AA),
		get_required<std::string>(vm, LEVEL)
	};
}

}
