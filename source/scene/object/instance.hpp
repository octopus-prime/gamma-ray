/*
 * instance.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include "surface/instance.hpp"
#include "texture/instance.hpp"

#include <rendering/ray.hpp>
#include <rendering/hit.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace object {

class instance_t
{
public:
	instance_t(surface::instance_t&& surface, texture::instance_t&& texture)
	:
		_surface(std::forward<surface::instance_t>(surface)),
		_texture(std::forward<texture::instance_t>(texture))
	{
	}

	rendering::hits_t::iterator
	hit(const rendering::ray_t& ray, const rendering::hits_t::iterator hits) const
	{
		const auto end = _surface->hit(ray, hits);
		std::for_each
		(
			hits, end,
			[this](rendering::hit_t& hit)
			{
				hit.object = this;
			}
		);
		return end;
	}

	const surface::instance_t&
	surface() const noexcept
	{
		return _surface;
	}

	const texture::instance_t&
	texture() const noexcept
	{
		return _texture;
	}

private:
	surface::instance_t _surface;
	texture::instance_t _texture;
};

typedef std::vector<instance_t> instances_t;

}
}
}
