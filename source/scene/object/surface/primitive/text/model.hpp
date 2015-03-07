/*
 * model.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/text/model_fwd.hpp>
#include <math/polynomial.hpp>
#include <geo/segment.hpp>
#include <geo/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace text {

class solve_contour_visitor
:
	public boost::static_visitor<>
{
	typedef boost::array<double, 3> roots_t;

	class check_range
	{
	public:
		constexpr bool
		operator()(const double x) const
		{
			return x >= 0.0 && x <= 1.0;
		}
	};

public:
	typedef boost::filter_iterator<check_range, roots_t::const_iterator> const_iterator;

	solve_contour_visitor(const float dx, const float dy, const float c)
	:
		boost::static_visitor<>(),
		_dx(dx),
		_dy(dy),
		_c(c),
		_roots(),
		_end()
	{
	}

	template <std::size_t N>
	void
	operator()(const basic_contour_t<N>& contour) const
	{
		const basic_polynomial_t<N> polynomial = transform(std::get<0>(contour));
		_end = solve(polynomial, _roots.begin());
	}

	const_iterator begin() const
	{
		return boost::make_filter_iterator<check_range>(_roots.cbegin(), _end);
	}

	const_iterator end() const
	{
		return boost::make_filter_iterator<check_range>(_end, _end);
	}

protected:
	template <std::size_t N>
	basic_polynomial_t<N>
	transform(const basic_bezier_t<N>& bezier) const
	{
		basic_polynomial_t<N> transformed;
		boost::transform
		(
			bezier,
			transformed.begin(),
			[this](const vector2_t& C)
			{
				return _dy * C[X] - _dx * C[Y];
			}
		);
		transformed[0] += _c;
		return transformed;
	}

private:
	float _dx, _dy, _c;
	mutable roots_t _roots;
	mutable roots_t::const_iterator _end;
};

template <std::size_t M>
class evaluate_contour_visitor
:
	public boost::static_visitor<vector2_t>
{
public:
	evaluate_contour_visitor(const float x)
	:
		boost::static_visitor<vector2_t>(),
		_x(x)
	{
	}

	template <std::size_t N>
	vector2_t
	operator()(const basic_contour_t<N>& contour) const
	{
		return evaluate(std::get<M>(contour), _x);
	}

private:
	float _x;
};

class glyph_t
{

public:
	glyph_t(contours_t&& contours, const float extrusion)
	:
		_contours(std::forward<contours_t>(contours)),
		_extrusion(extrusion)
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		if (std::fabs(ray.direction()[X]) < std::numeric_limits<float>::epsilon() && std::fabs(ray.direction()[Y]) < std::numeric_limits<float>::epsilon())
			return hits;

		auto end = hits;

		const solve_contour_visitor solver
		(
			ray.direction()[X],
			ray.direction()[Y],
			ray.origin()[Y] * ray.direction()[X] - ray.origin()[X] * ray.direction()[Y]
		);

		for (const contour_t& contour : _contours)
		{
			boost::apply_visitor(solver, contour);
			for (const double root : solver)
			{
				const evaluate_contour_visitor<0> function_evaluater(root);
				const vector2_t point = boost::apply_visitor(function_evaluater, contour);
				const float t = std::abs(ray.direction()[X]) > std::abs(ray.direction()[Y]) ?
						(point[X] - ray.origin()[X]) / ray.direction()[X] :
						(point[Y] - ray.origin()[Y]) / ray.direction()[Y];
				if (ray[t])
				{
					const float z = ray.origin()[Z] + ray.direction()[Z] * t;
					if (z >= 0.0 && z <= _extrusion)
					{
						const evaluate_contour_visitor<1> derivation_evaluater(root);
						const vector2_t normal = boost::apply_visitor(derivation_evaluater, contour);
						end->distance = t;
						end->normal = vector3_t {{-normal[Y], normal[X], 0.0f}};
						++end;
					}
				}
			}
		}

		return end;
	}

	bool
	inside(const vector3_t& point) const
	{
		const solve_contour_visitor solver(1.0f, 1.0f, point[Y] - point[X]);

		return boost::accumulate
		(
			_contours,
			0,
			[&point, &solver](const std::size_t crossings, const contour_t& contour)
			{
				boost::apply_visitor(solver, contour);

				return boost::accumulate
				(
					solver,
					crossings,
					[&point, &contour](const std::size_t crossings, const double root)
					{
						const evaluate_contour_visitor<0> evaluater(root);
						const vector2_t p = boost::apply_visitor(evaluater, contour);
						return crossings + (p[X] > point[X] && p[Y] > point[Y]);
					}
				);
			}
		) & 1;	// odd = inside, even = outside
	}

private:
	contours_t _contours;
	float _extrusion;
};

typedef std::vector<glyph_t> glyphs_t;
typedef std::pair<box_t, boost::uint32_t> value_t;
typedef geo::index::rtree<value_t, geo::index::rstar<8>> rtree_t; // TODO: parse parameter

class model
{
public:
	model(glyphs_t&& glyphs, rtree_t&& rtree, const float extrusion)
	:
		_glyphs(std::forward<glyphs_t>(glyphs)),
		_rtree(std::forward<rtree_t>(rtree)),
		_extrusion(extrusion)
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		auto end = hits;

		if (std::fabs(ray.direction()[Z]) > std::numeric_limits<float>::epsilon())
		{
			const float distance1 = -ray.origin()[Z] / ray.direction()[Z];
			if (ray[distance1])
			{
				const vector3_t point = ray(distance1);
				const auto test = [this, &point, &end, distance1](const value_t& value)
				{
					const glyph_t& glyph = _glyphs[value.second];
					if (glyph.inside(point))
					{
						end->distance = distance1;
						end->normal = vector3_t {{0, 0, -1}};
						++end;
					}
					return false;
				};

				_rtree.query(geo::index::intersects(point) && geo::index::satisfies(test), (value_t*) nullptr);
			}

			const float distance2 = (_extrusion - ray.origin()[Z]) / ray.direction()[Z];
			if (ray[distance2])
			{
				const vector3_t point = ray(distance2);
				const auto test = [this, &point, &end, distance2](const value_t& value)
				{
					const glyph_t& glyph = _glyphs[value.second];
					if (glyph.inside(point))
					{
						end->distance = distance2;
						end->normal = vector3_t {{0, 0, +1}};
						++end;
					}
					return false;
				};

				_rtree.query(geo::index::intersects(point) && geo::index::satisfies(test), (value_t*) nullptr);
			}
		}

		const auto test = [this, &ray, &end](const value_t& value)
		{
			const glyph_t& glyph = _glyphs[value.second];
			end = glyph.hit(ray, end);
			return false;
		};

		const segment_t segment = ray;
		_rtree.query(geo::index::intersects(segment) && geo::index::satisfies(test), (value_t*) nullptr);

		return end;
	}

	bool
	inside(const vector3_t& point) const
	{
		// TODO: use rtree !
		return boost::algorithm::any_of
		(
			_glyphs,
			std::bind
			(
				&glyph_t::inside,
				std::placeholders::_1,
				point
			)
		);
	}

private:
	glyphs_t _glyphs;
	rtree_t _rtree;
	float _extrusion;
};

}
}
}
}
}
}
