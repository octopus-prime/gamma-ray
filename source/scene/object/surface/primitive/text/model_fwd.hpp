/*
 * model_fwd.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/bezier.hpp>
#include <boost/variant.hpp>
#include <vector>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace text {

template <std::size_t N>
using basic_contour_t = std::tuple<basic_bezier_t<N>, basic_bezier_t<N-1>>;

typedef basic_contour_t<2> contour1_t;
typedef basic_contour_t<3> contour2_t;
typedef basic_contour_t<4> contour3_t;

typedef boost::variant<contour1_t, contour2_t, contour3_t> contour_t;
typedef std::vector<contour_t> contours_t;

}
}
}
}
}
}
