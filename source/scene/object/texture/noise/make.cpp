/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/texture/noise/make.hpp>
#include <libnoise/module/module.h>
#include <boost/algorithm/clamp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace module = noise::module;

namespace rt {
namespace scene {
namespace object {
namespace texture {
namespace noise {

typedef boost::shared_ptr<module::Module> module_t;

template <typename Module, std::size_t N>
class module_adapter
:
	public Module
{
public:
	template <typename... Args>
	module_adapter(Args&&... modules)
	:
		Module(),
		_modules {{std::forward<module_t>(modules)...}}
	{
		for (std::size_t i = 0; i < N; ++i)
			Module::SetSourceModule(i, *_modules[i]);
	}

private:
	std::array<module_t, N> _modules;
};

template <typename Module, std::size_t N, typename... Args>
boost::shared_ptr<Module>
make(Args&&... modules)
{
	return boost::make_shared<module_adapter<Module, N>>(std::forward<module_t>(modules)...);
}

class make_visitor
:
	public boost::static_visitor<module_t>
{
public:
	template <typename Description>
	result_type operator()(const Description& description) const
	{
		return result_type();
	}

	result_type operator()(const double description) const
	{
		const auto module = make<module::Const, 0>();
		module->SetConstValue(description);
		return module;
	}

	result_type operator()(const generator::cylinders::description_t& description) const
	{
		const auto module = make<module::Cylinders, 0>();
		if (description->frequency)
			module->SetFrequency(*description->frequency);
		return module;
	}

	result_type operator()(const generator::perlin::description_t& description) const
	{
		const auto module = make<module::Perlin, 0>();
		if (description->frequency)
			module->SetFrequency(*description->frequency);
		// todo...
		return module;
	}

	result_type operator()(const combiner::description_t<combiner::add_tag>& description) const
	{
		const auto module = make<module::Add, 2>
		(
			boost::apply_visitor(*this, description->noise1),
			boost::apply_visitor(*this, description->noise2)
		);
		return module;
	}

	result_type operator()(const combiner::description_t<combiner::mul_tag>& description) const
	{
		const auto module = make<module::Multiply, 2>
		(
			boost::apply_visitor(*this, description->noise1),
			boost::apply_visitor(*this, description->noise2)
		);
		return module;
	}
};

instance_t
make(const description_t& description)
{
	const module_t module = boost::apply_visitor(make_visitor(), description);
	return [module](const vector3_t& point)
	{
		return boost::algorithm::clamp(module->GetValue(point[X], point[Y], point[Z]) * 0.5 + 0.5, 0, 1);
	};
}

}
}
}
}
}
