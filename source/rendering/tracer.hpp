/*
 * tracer.hpp
 *
 *  Created on: 09.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <scene/instance.hpp>

namespace rt {
namespace rendering {

class tracer_t
{
public:
	tracer_t(const scene::instance_t& scene, const std::size_t max_hits)
	:
		_scene(scene),
		_hits(10 * max_hits)
	{
	}

	vector3_t
	operator()(const ray_t& ray, const std::size_t depth) const
	{
		constexpr vector3_t ambient = {{1,1,1}};

		const hits_t::const_iterator end = _scene.hit(ray, _hits.begin());
		if (end == _hits.begin())
			return {{0,0,0}};
		const hit_t& hit = *_hits.cbegin();

		const scene::object::texture::instance_t& texture = hit.object->texture();
		const vector3_t point = hit.point;
		const vector3_t N = normalize(hit.normal + texture.bump(point));
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
				if (depth > 0 && _scene.hit(ray_t(point, L, 0, LPl), _hits.begin()) != _hits.begin()) // shadow test
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

			if (t > 0.0f)
			{
				const float DN = ray.direction() * N;
				const vector3_t R = ray.direction() - 2.0f * DN * N;
				const float n = texture.refraction(point);
				if (DN < 0)	// in
				{
					const float D = 1 - (1 - DN * DN) / (n * n);
					const vector3_t T = (ray.direction() - N * DN) / n - N * std::sqrt(D);
					I += (*this)(point, R, T, n, -DN, t, depth - 1);
				}
				else // out
				{
					const float D = 1 - (1 - DN * DN) * (n * n);
					if (D > 0)
					{
						static const float a = -std::log(0.9f);
						const vector3_t T = (ray.direction() - N * DN) * n + N * std::sqrt(D);
						const float k = std::exp(a * static_cast<float>(hit.distance)) * t;
						I += (*this)(point, R, T, n, T * N, k, depth - 1);
					}
					else
						I += (*this)(ray_t(point, R), depth - 1) * t;
				}
			}
		}

		return I;
	}

protected:
	vector3_t
	operator()(const vector3_t& point, const vector3_t& R, const vector3_t& T, const float n, const float c, const float k, const std::size_t depth) const
	{
		const float R0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));
		const float R1 = R0 + (1 - R0) * std::pow(1 - c, 5);
		return ((*this)(ray_t(point, R), depth) * R1 + (*this)(ray_t(point, T), depth) * (1 - R1)) * k;
	}

private:
	const scene::instance_t& _scene;
	mutable hits_t _hits;
};

}
}
