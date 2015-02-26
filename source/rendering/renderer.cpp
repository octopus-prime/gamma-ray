/*
 * renderer.cpp
 *
 *  Created on: 11.01.2014
 *      Author: mike_gresens
 */

#include <rendering/renderer.hpp>
#include <rendering/tracer.hpp>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>

namespace rt {
namespace rendering {

renderer_t::renderer_t(const scene::instance_t& scene, const AA aa, const std::size_t depth)
:
	_scene(scene),
	_deltas(DELTAS.at(static_cast<std::size_t>(aa))),
	_depth(depth),
	_max(max(_scene.objects()))
{
}

void
renderer_t::operator()(view_t view) const
{
	const float factor = float(view.width()) / float(view.height());
	const vector2_t increment {{factor / view.width(), 1.0f / view.height() }};
	tbb::parallel_for
	(
		tbb::blocked_range2d<coord_t>(0, view.height(), 0, view.width()),
		[&view, this, &increment, factor](const tbb::blocked_range2d<coord_t>& range)
		{
			const tracer_t trace(_scene, _max);
			for (coord_t y = range.rows().begin(); y != range.rows().end(); ++y)
			{
				for (coord_t x = range.cols().begin(); x != range.cols().end(); ++x)
				{
					const vector2_t point{{float(x) * increment[X] - 0.5f * factor, float(y) * increment[Y] - 0.5f}};
					view(view.width() - x - 1, view.height() - y - 1) = to_pixel(render(point, increment, trace));
				}
			}
		}
	);
}

vector3_t
renderer_t::render(const vector2_t& point, const vector2_t& inc, const tracer_t& trace) const
{
	return boost::accumulate
	(
		_deltas,
		vector3_t {{0,0,0}},
		[&point, this, &inc, &trace](const vector3_t& pixel, const vector2_t& delta) -> vector3_t
		{
			const ray_t ray = _scene.camera()(point + (delta & inc));
			return pixel + trace(ray, _depth);
		}
	) / _deltas.size();
}

inline pixel_t
renderer_t::to_pixel(const vector3_t& color)
{
	return pixel_t
	(
		1.0f - std::exp(-color[0]),
		1.0f - std::exp(-color[1]),
		1.0f - std::exp(-color[2])
	);
}

inline std::size_t
renderer_t::max(const scene::object::instances_t& objects)
{
	/*
	return boost::accumulate
	(
		objects,
		0,
		std::bind
		(
			std::plus<std::size_t>(),
			std::placeholders::_1,
			std::bind(&scene::base_object_t::max, std::placeholders::_2)
		)
	);
	*/
	return 100;
}

const std::unordered_map<std::size_t, std::vector<vector2_t>>
renderer_t::DELTAS
{
	{
		static_cast<std::size_t>(AA::aa1),
		{{
			{{+0.0f, +0.0f}}
		}}
	},
	{
		static_cast<std::size_t>(AA::aa4),
		{{
			{{-0.4f, +0.1f}},
			{{+0.1f, +0.4f}},
			{{+0.4f, -0.1f}},
			{{-0.1f, -0.4f}}
		}}
	},
	{
		static_cast<std::size_t>(AA::aa5),
		{{
			{{+0.0f, +0.0f}},
			{{-0.4f, +0.1f}},
			{{+0.1f, +0.4f}},
			{{+0.4f, -0.1f}},
			{{-0.1f, -0.4f}}
		}}
	},
	{
		static_cast<std::size_t>(AA::aa9),
		{{
			{{-0.4f, +0.1f}},
			{{+0.1f, +0.4f}},
			{{+0.4f, -0.1f}},
			{{-0.1f, -0.4f}},
			{{+0.0f, +0.0f}},
			{{-0.3f, +0.2f}},
			{{+0.2f, +0.3f}},
			{{+0.3f, -0.2f}},
			{{-0.2f, -0.3f}}
		}}
	}
};

}
}
