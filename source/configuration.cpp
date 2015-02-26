/*
 * configuration.cpp
 *
 *  Created on: 25.02.2015
 *      Author: mike_gresens
 */

#include "configuration.hpp"
#include <boost/program_options/parsers.hpp>
#include <fstream>
#include <iostream>

namespace rt {

template <typename T>
T
get_required(const po::variables_map& vm, const std::string& option)
{
	const auto value = vm.find(option);
	if (value == vm.end())
		throw std::runtime_error("Missing option: " + option);
	return value->second.as<T>();
}

template <typename T>
boost::optional<T>
get_optional(const po::variables_map& vm, const std::string& option)
{
	const auto value = vm.find(option);
	if (value == vm.end())
		return boost::none;
	return value->second.as<T>();
}

template <>
std::string
configuration::get_required(const std::string& option) const
{
	return rt::get_required<std::string>(_vm, option);
}

template <>
std::size_t
configuration::get_required(const std::string& option) const
{
	return rt::get_required<std::size_t>(_vm, option);
}

template <>
boost::optional<std::string>
configuration::get_optional(const std::string& option) const
{
	return rt::get_optional<std::string>(_vm, option);
}

configuration::configuration(const int argc, const char* const argv[])
:
	_options(),
	_vm()
{
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
	;

	_options.add(actions);
	_options.add(options);

	po::store(po::parse_command_line(argc, argv, _options), _vm);
	po::notify(_vm);

	if (argc == 1 || _vm.empty() || _vm.count(HELP))
	{
		std::cout << std::endl;
		std::cout << "Usage: " << argv[0] << " <action> <options>" << std::endl;
		std::cout << _options << std::endl;
		exit(_vm.count(HELP) == 1);
	}

	const auto config = get_optional<std::string>(CONFIG);
	if (config)
	{
		std::ifstream file(*config);
		if (file.fail())
			throw std::runtime_error(*config + " not found.");
		po::store(po::parse_config_file(file, _options), _vm);
		file.close();
		po::notify(_vm);
	}
}

const std::string configuration::HELP("help");
const std::string configuration::CONFIG("config");
const std::string configuration::INPUT("input");
const std::string configuration::OUTPUT("output");
const std::string configuration::WIDTH("width");
const std::string configuration::HEIGHT("height");
const std::string configuration::DEPTH("depth");
const std::string configuration::AA("aa");

}
