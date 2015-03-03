/*
 * bar.cpp
 *
 *  Created on: 03.03.2015
 *      Author: mgresens
 */

#include <math/vector.hpp>
#include <geo/segment.hpp>
#include <geo/box.hpp>

namespace rt {

void bar()
{
	segment_t s1;
	box_t b1;
	box_t b2;
	box_t b3;

	geo::intersects(b1, s1);
	geo::intersects(b1, b2);

	geo::intersection(b1, b2, b3);
}

}
