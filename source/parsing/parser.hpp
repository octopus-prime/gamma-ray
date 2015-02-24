/*
 * parser.hpp
 *
 *  Created on: 07.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/description.hpp>
#include <iostream>

namespace rt {
namespace parsing {

scene::description_t parse(const std::string& file);

}
}
