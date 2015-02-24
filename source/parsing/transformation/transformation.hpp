/*
 * transformation.hpp
 *
 *  Created on: 21.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <parsing/transformation/description.hpp>

namespace rt {
namespace parsing {
namespace transformation {

void translate(const description_t& transformable, const vector3_t& translation);
void rotate(const description_t& transformable, const vector3_t& rotation);
void scale(const description_t& transformable, const vector3_t& scaling);

}
}
}
