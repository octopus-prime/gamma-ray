/*
 * tracer.hpp
 *
 *  Created on: 09.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <scene/instance.hpp>
#include <vector>

#include <boost/bind.hpp>
#include <boost/optional.hpp>

#include <scene/object/texture/instance.hpp>
#include <libnoise/module/perlin.h>
#include <boost/make_shared.hpp>
#include <boost/algorithm/clamp.hpp>

namespace rt {
namespace rendering {
/*
static const noise::module::Perlin perlin;// = boost::make_shared<noise::module::Perlin>();

float noise(const vector3_t& point)
{
	return boost::algorithm::clamp(perlin.GetValue(point[X], point[Y], point[Z]) * 0.5 + 0.5, 0, 1);
}

const scene::object::texture::instance_t texture
(
	// pigment
	[](const vector3_t& point) -> vector3_t
	{
//		const float value = noise(point);
		return
		{{
			1,0,0
//			value,
//			value,
//			value
//			noise(point),
//			noise(point),
//			noise(point)
		}};
	},
	// bump
	[](const vector3_t& point) -> vector3_t
	{
		return
		{{
			0,
			0,
			0
		}};
	},
	// ambient
	[](const vector3_t& point) -> float
	{
//		return perlin.GetValue(point[X], point[Y], point[Z]);
//		return 0.3;
		return noise(point);
	},
	// diffuse
	[](const vector3_t& point) -> float
	{
		return 0.5;
	},
	// specular
	[](const vector3_t& point) -> float
	{
		return 0.7;
	},
	// shininess
	[](const vector3_t& point) -> float
	{
		return 164;
	},
	// reflection
	[](const vector3_t& point) -> float
	{
		return 0.8;
	},
	// transparency
	[](const vector3_t& point) -> float
	{
		return 0.0;
	},
	// refraction
	[](const vector3_t& point) -> float
	{
		return 0.0;
	}
);
*/
class tracer_t
{
public:
	tracer_t(const scene::instance_t& scene, const std::size_t max_hits)
	:
		_scene(scene),
		_hits()//max_hits)
	{
	}

	vector3_t
	operator()(const ray_t& ray, const std::size_t depth) const
	{
		constexpr vector3_t ambient = {{1,1,1}};
		const hits_t::const_iterator end = find_hits(ray, _hits.begin());
		const auto hit = std::min_element(_hits.cbegin(), end);
		if (hit == end)
			return {{0,0,0}};

		const scene::object::texture::instance_t& texture = hit->object->texture();
		const vector3_t point = hit->point;
		const vector3_t N = normalize(hit->normal + texture.bump(point));
//		const float factor = (texture.shininess(point) + 2.f) / (8.f * M_PI);
		const float factor = (texture.shininess(point) + 2.f) / (2.f * M_PI);
		const vector3_t color = texture.pigment(point);

		vector3_t I = boost::accumulate
		(
			_scene.lights(),
			(color & ambient) * texture.ambient(point),
			[&](vector3_t I, const scene::light::instance_t& light)
			{
				const vector3_t LP = light.origin() - point;
				const float LPl = length(LP);
				const vector3_t L = LP / LPl;
				if (shadow(ray_t(point, L, 0, LPl)))
					return I;
				const vector3_t H = normalize(L - ray.direction());
				const vector3_t d = std::max(0.0f, L * N) * texture.diffuse(point) * color;
				const vector3_t s = factor * std::pow(std::max(0.0f, N * H), texture.shininess(point)) * texture.specular(point) * ambient; // * light.color();
				return I + ((d + s) & light.color() / (LPl * LPl));
			}
		);

		if (depth > 0)
		{
			const float t = texture.transparency(point);
			const float r = texture.reflection(point);
			if (r > 0 && t == 0)
			{
				const vector3_t R = ray.direction() - 2.0f * (ray.direction() * N) * N;
				I += (*this)(ray_t(point, R), depth - 1) * r;
			}
/*
			const float t = texture->transparency(point);
			if (t > 0.0f)
			{
				const float DN = direction * N;
				if (DN < 0)	// in
				{
					const float r = 1.0f / texture->refraction(point);
					const float c = std::sqrt(1.f - r * r * (1.f - DN * DN));
					if (c >= 0)
					{
						const vector3_t T = r * direction - (r * DN + c) * N;
						I += (*this)(point, T, depth - 1) * t;
					}
				}
				else // out
				{
					const float r = texture->refraction(point);
					const float c = std::sqrt(1.f - r * r * (1.f - DN * DN));
					if (c >= 0)
					{
						const vector3_t T = r * direction - (r * DN - c) * N;
						I += (*this)(point, T, depth - 1) * t;
					}
				}
			}
*/
//			if (t > 0.0f)
//			{
//				const float DN = direction * N;
//				const vector3_t R = direction - 2.0f * DN * N;
//				const float n = texture->refraction(point);
//				if (DN < 0)	// in
//				{
//					const auto T = refract(-direction, N, 1 / n);
//					if (!!T)
//					{
//					const float c = -DN;
//					const float R0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));
//					const float R1 = R0 + (1 - R0) * std::pow(1 - c, 5);
//					I += ((*this)(point, R, depth - 1) * R1 + (*this)(point, T.get(), depth - 1) * (1 - R1))*t;
//					}
//					else
//					{
//						I += (*this)(point, R, depth - 1)*t;
//					}
//				}
//				else // out
//				{
//					const auto T = refract(-direction, -N, 1);
//					if (!T)
//					{
//						I += (*this)(point, R, depth - 1) *t;
//					}
//					else
//					{
//						static const scene::color_t a {{std::log(0.91f), std::log(0.90f), std::log(0.89f)}};
//						const scene::color_t k
//						{{
//							std::exp(-a[X] * static_cast<float>(hit->distance)),
//							std::exp(-a[Y] * static_cast<float>(hit->distance)),
//							std::exp(-a[Z] * static_cast<float>(hit->distance))
//						}};
//						const float c = T.get() * N;
//						const float R0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));
//						const float R1 = R0 + (1 - R0) * std::pow(1 - c, 5);
//						I += (((*this)(point, R, depth - 1) * R1 + (*this)(point, T.get(), depth - 1) * (1 - R1)) & k) * t;
//					}
//				}
//			}
		}

		return I;
	}

protected:
	boost::optional<vector3_t>
	refract(const vector3_t& direction, const vector3_t& normal, const float n) const
	{
		const float DN = direction * normal;
		const float D = 1 - n * n * (1 - DN * DN);

		if (D <= 0)
			return boost::none;

		return n * (direction - normal * DN) - normal * std::sqrt(D);
	}

	bool
	shadow(const ray_t& ray) const
	{
		return std::any_of
		(
			_scene.objects().cbegin(), _scene.objects().cend(),
			std::bind
			(
				std::not_equal_to<hits_t::const_iterator>(),
				std::bind(&scene::object::instance_t::hit, std::placeholders::_1, ray, _hits.begin()),
				_hits.cbegin()
			)
		);
	}

	hits_t::iterator
	find_hits(const ray_t& ray, const hits_t::iterator hits) const
	{
		return std::accumulate
		(
			_scene.objects().cbegin(), _scene.objects().cend(),
			hits,
			std::bind(&scene::object::instance_t::hit, std::placeholders::_2, ray, std::placeholders::_1)
		);
	}

private:
	const scene::instance_t& _scene;
	mutable hits_t _hits;
};

//constexpr float tracer_t::epsilon;

}
}