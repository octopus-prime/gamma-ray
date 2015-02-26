/*
 * description.hpp
 *
 *  Created on: 16.02.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/texture/noise/generator/perlin/description.hpp>
#include <scene/object/texture/noise/generator/billow/description.hpp>
#include <scene/object/texture/noise/generator/cylinders/description.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace texture {
namespace noise {

namespace combiner {

struct add_tag;
struct mul_tag;
struct max_tag;
struct min_tag;
struct pow_tag;

template <typename Operation>
struct basic_description_t;

template <typename Operation>
using description_t = boost::shared_ptr<basic_description_t<Operation>>;

}

typedef boost::variant
<
	double, // for constant
	generator::perlin::description_t,
	generator::billow::description_t,
	generator::cylinders::description_t,
	boost::recursive_wrapper<combiner::description_t<combiner::add_tag>>,
	boost::recursive_wrapper<combiner::description_t<combiner::mul_tag>>,
	boost::recursive_wrapper<combiner::description_t<combiner::max_tag>>,
	boost::recursive_wrapper<combiner::description_t<combiner::min_tag>>,
	boost::recursive_wrapper<combiner::description_t<combiner::pow_tag>>
> description_t;

namespace combiner {

template <typename Operation>
struct basic_description_t
{
	basic_description_t(const noise::description_t& noise1, const noise::description_t& noise2)
	:
		noise1(noise1),
		noise2(noise2)
	{
	}

	noise::description_t noise1;
	noise::description_t noise2;
};

}

}
}
}
}
}
