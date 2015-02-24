/*
 * instance.hpp
 *
 *  Created on: 24.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/camera/instance.hpp>
#include <scene/light/instance.hpp>
#include <scene/object/instance.hpp>

namespace rt {
namespace scene {

class instance_t
{
public:
	instance_t(camera::instance_t&& camera, light::instances_t&& lights, object::instances_t&& objects)
	:
		_camera(camera),
		_lights(lights),
		_objects(objects)
	{
	}

	const camera::instance_t&
	camera() const noexcept
	{
		return _camera;
	}

	const light::instances_t&
	lights() const noexcept
	{
		return _lights;
	}

	const object::instances_t&
	objects() const noexcept
	{
		return _objects;
	}

private:
	camera::instance_t _camera;
	light::instances_t _lights;
	object::instances_t _objects;
};

}
}
