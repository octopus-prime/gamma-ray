/*
 * configuration.hpp
 *
 *  Created on: 25.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <boost/program_options.hpp>
#include <boost/optional.hpp>

namespace po = boost::program_options;

namespace rt {

class configuration
{
public:
	static const std::string INPUT;
	static const std::string OUTPUT;
	static const std::string WIDTH;
	static const std::string HEIGHT;
	static const std::string DEPTH;
	static const std::string AA;

	configuration(const int argc, const char* const argv[]);

	template <typename T>
	T
	get_required(const std::string& option) const;

	template <typename T>
	boost::optional<T>
	get_optional(const std::string& option) const;

private:
	static const std::string HELP;
	static const std::string CONFIG;

	po::options_description _options;
	po::variables_map _vm;
};

}
