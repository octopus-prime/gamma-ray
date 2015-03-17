/*
 * keywords.cpp
 *
 *  Created on: 17.03.2015
 *      Author: mike_gresens
 */

#include "keywords.hpp"

namespace rt {
namespace parsing {

const boost::array<std::string, 13> keywords
{{
	"Int",
	"Float",
	"String",
	"Vector2",
	"Vector3",
	"Vector4",
	"Scene",
	"Camera",
	"Light",
	"Object",
	"Surface",
	"Texture",
	"Noise"
}};

}
}
