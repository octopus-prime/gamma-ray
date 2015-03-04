/*
 * box.hpp
 *
 *  Created on: 03.03.2015
 *      Author: mgresens
 */

#pragma once

#include <geo/point.hpp>
#include <math/matrix.hpp>
#include <math/homogeneous.hpp>
#include <vector>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>

namespace rt {

typedef geo::model::box<vector3_t> box_t;

inline box_t
transform(const matrix44_t& transformation, const box_t& box)
{
	const matrix44_t matrix = invert(transformation);
	const vector3_t& min = box.min_corner();
	const vector3_t& max = box.max_corner();

	geo::model::linestring<vector3_t> points;
	points.push_back({{min[X], min[Y], min[Z]}});
	points.push_back({{min[X], min[Y], max[Z]}});
	points.push_back({{min[X], max[Y], min[Z]}});
	points.push_back({{min[X], max[Y], max[Z]}});
	points.push_back({{max[X], min[Y], min[Z]}});
	points.push_back({{max[X], min[Y], max[Z]}});
	points.push_back({{max[X], max[Y], min[Z]}});
	points.push_back({{max[X], max[Y], max[Z]}});

	boost::transform
	(
		points,
		points.begin(),
		[&matrix](const vector3_t& point)
		{
			return point::operator*(matrix, point);
		}
	);

//	boost::for_each // TODO: hack :(
//	(
//		points,
//		[](vector3_t& point)
//		{
//			boost::for_each
//			(
//				point,
//				[](float& f)
//				{
//					if (f > +1e10f)
//						f = +1e10f;
//					if (f < -1e10f)
//						f = -1e10f;
//				}
//			);
//		}
//	);

	return geo::return_envelope<box_t>(points);
}

}
