/*
 * reader.hpp
 *
 *  Created on: 07.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <string>

namespace rt {
namespace parsing {

template <typename Parser, typename Attribute>
bool read(const std::string& file, Parser& parser, Attribute& attribute);

}
}
