/*
 * reader.hpp
 *
 *  Created on: 02.03.2015
 *      Author: mike_gresens
 */

#pragma once

#include <scene/object/surface/primitive/mesh/model_fwd.hpp>
#include <rply.h>
#include <boost/shared_ptr.hpp>

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {
namespace mesh {

class reader_t
{
public:
	reader_t(const boost::shared_ptr<t_ply_> ply);
	void operator()(vertexes_t& vertexes, faces_t& faces) const;

protected:
	static int vertex_cb(p_ply_argument argument);
	static int face_cb(p_ply_argument argument);

private:
	boost::shared_ptr<t_ply_> _ply;
};

reader_t
open(const std::string& file);

}
}
}
}
}
}
