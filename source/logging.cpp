/*
 * logging.cpp
 *
 *  Created on: 05.03.2015
 *      Author: mike_gresens
 */

#include "logging.hpp"
#include <unordered_map>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/formatters.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

using namespace boost::log;

namespace rt {

static const std::unordered_map<char, trivial::severity_level> LEVEL
{
	{'t', trivial::trace},
	{'d', trivial::debug},
	{'i', trivial::info},
	{'w', trivial::warning},
	{'e', trivial::error},
	{'f', trivial::fatal}
};

void
logging(const char level)
{
	register_simple_formatter_factory<trivial::severity_level, char>("Severity");
	core::get()->add_global_attribute("Duration", attributes::timer());
	core::get()->add_global_attribute("Scope", attributes::named_scope());
	core::get()->set_filter(trivial::severity >= LEVEL.at(level));
	add_console_log(std::clog, keywords::format = "%Duration% [%Scope%] <%Severity%> %Message%");
	/*
	formatter formatter =
		expressions::stream
			<< expressions::format_date_time< boost::posix_time::time_duration >("Duration", "%H:%M:%S.%f") << " *"
			<< std::left << std::setw(20)
			<< expressions::format_named_scope("Scope", keywords::format = "[%n]") << ": "
	//            << expressions::attr<trivial::severity_level>("Severity") << "* "
			<< expressions::message;

//	add_console_log()->set_formatter(formatter);
	//	add_console_log()->set_filter(trivial::severity >= LEVEL.at(level));
	*/
}

}
