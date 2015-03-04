/*
 * renderer.hpp
 *
 *  Created on: 09.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <scene/instance.hpp>
//#include <scene/texture/color.hpp>
#include <rendering/image.hpp>
#include <vector>
#include <unordered_map>

namespace rt {
namespace rendering {

class tracer_t;

enum class AA
{
	aa1 = 1,
	aa4 = 4,
	aa5 = 5,
	aa9 = 9
};

class renderer_t
{
public:
	renderer_t(const scene::instance_t& scene, const AA aa, const std::size_t depth);
	void operator()(view_t view) const;

protected:
	vector3_t render(const vector2_t& point, const vector2_t& inc, const tracer_t& trace) const;
	static inline pixel_t to_pixel(const vector3_t& color);
//	static std::size_t max(const scene::object::instances_t& objects);

private:
	const scene::instance_t& _scene;
	const std::vector<vector2_t>& _deltas;
	std::size_t _depth;
	std::size_t _max;

	static const std::unordered_map<std::size_t, std::vector<vector2_t>> DELTAS;
};

}
}
