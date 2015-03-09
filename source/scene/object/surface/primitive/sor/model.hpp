/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/spline.hpp>
#include <geo/segment.hpp>
#include <geo/box.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace sor {

typedef std::vector<polynomial5_t> derivations_t;
typedef std::pair<box_t, boost::uint32_t> value_t;
typedef geo::index::rtree<value_t, geo::index::rstar<8>> rtree_t; // TODO: parse parameter

class model
{
public:
	model(spline_t&& spline, derivations_t&& derivations, rtree_t&& rtree, const float heightBase, const float heightCap, const float radiusBase, const float radiusCap)
	:
		_spline(std::forward<spline_t>(spline)),
		_derivations(std::forward<derivations_t>(derivations)),
		_rtree(std::forward<rtree_t>(rtree)),
		_heightBase(heightBase),
		_heightCap(heightCap),
		_radiusBase(radiusBase),
		_radiusCap(radiusCap)
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		auto end = hits;

		// Test base plane
		const float tB = (_heightBase - ray.origin()[Y]) / ray.direction()[Y];
		if (ray[tB])
		{
			const float uB = ray.origin()[X] + tB * ray.direction()[X];
			const float vB = ray.origin()[Z] + tB * ray.direction()[Z];
			const float rB = uB * uB + vB * vB;
			if (rB <= _radiusBase * _radiusBase)
			{
				end->distance = tB;
				end->normal = vector3_t {{0, -1, 0}};
				end++;
			}
		}

		// Test cap plane
		const float tC = (_heightCap - ray.origin()[Y]) / ray.direction()[Y];
		if (ray[tC])
		{
			const float uC = ray.origin()[X] + tC * ray.direction()[X];
			const float vC = ray.origin()[Z] + tC * ray.direction()[Z];
			const float rC = uC * uC + vC * vC;
			if (rC <= _radiusCap * _radiusCap)
			{
				end->distance = tC;
				end->normal = vector3_t {{0, +1, 0}};
				end++;
			}
		}

		const polynomial2_t d
		{{
			ray.origin()[X] * ray.origin()[X] + ray.origin()[Z] * ray.origin()[Z],
			2.0 * (ray.origin()[X] * ray.direction()[X] + ray.origin()[Z] * ray.direction()[Z]),
			ray.direction()[X] * ray.direction()[X] + ray.direction()[Z] * ray.direction()[Z]
		}};

		const auto test = [this, &ray, &d, &end](const value_t& value)
		{
			const spline_segment_t& segment = _spline[value.second];

			const float y = ray.origin()[Y] - segment.get<0>();
			const polynomial3_t& s = segment.get<2>();

			const polynomial3_t p
			{{
				y * (y * (s[3] * y + s[2]) + s[1]) + s[0],
				ray.direction()[Y] * (y * (3.0 * s[3] * y + 2.0 * s[2]) + s[1]),
				ray.direction()[Y] * ray.direction()[Y] * (3.0 * s[3] * y + s[2]),
				s[3] * ray.direction()[Y] * ray.direction()[Y] * ray.direction()[Y]
			}};

//			const polynomial6_t q = p * p - d;
			polynomial6_t q = p * p;
			boost::transform(d, q, q.begin(), [](const double d, const double q){return q - d;});

			const rendering::distance_iterator roots_begin(end);
			const rendering::distance_iterator roots_end = solve(q, roots_begin);
			rendering::hits_t::iterator out = end;
			std::for_each
			(
				end, end + std::distance(roots_begin, roots_end),
				[this, &ray, &segment, &out](const rendering::hit_t& hit)
				{
					if (ray[hit.distance])
					{
						const vector3_t point = ray(hit.distance);
						if (point[Y] >= segment.get<0>() && point[Y] <= segment.get<1>())
						{
							out->distance = hit.distance;
							out->normal = normal(point);
							out++;
						}
					}
				}
			);
			end = out;

			return false;
		};

		const segment_t segment = ray;
		_rtree.query(geo::index::intersects(segment) && geo::index::satisfies(test), (value_t*) nullptr);

		return end;
	}

	bool
	inside(const vector3_t& point) const
	{
		if (point[Y] < _heightBase || point[Y] > _heightCap)
			return false;

		return std::hypot(point[X], point[Z]) <= evaluate(_spline, point[Y]);
	}

protected:
	vector3_t
	normal(const vector3_t& point) const
	{
		const spline_t::const_iterator segment = boost::lower_bound
		(
			_spline, point[Y],
			[](const spline_segment_t& segment, const float x)
			{
				return segment.get<1>() < x;
			}
		);

		const auto derivation = _derivations.cbegin() + std::distance(_spline.cbegin(), segment);

		return
		{{
			point[X],
			-0.5f * float(evaluate(*derivation, point[Y] - segment->get<0>())),
			point[Z]
		}};
	}

private:
	spline_t _spline;
	derivations_t _derivations;
	rtree_t _rtree;
	float _heightBase;
	float _heightCap;
	float _radiusBase;
	float _radiusCap;
};

}
}
}
}
}
}
