/*
 * model_fwd.hpp
 *
 *  Created on: 04.01.2014
 *      Author: mike_gresens
 */

#pragma once

#include <math/vector.hpp>
#include <boost/cstdint.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace mesh {

typedef std::vector<vector3_t> vertexes_t;
typedef std::vector<vector3_t> normals_t;
typedef boost::array<boost::uint32_t, 3> face_t;
typedef std::vector<face_t> faces_t;

}
}
}
}
}
}
