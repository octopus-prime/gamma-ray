/*
 * image.hpp
 *
 *  Created on: 24.05.2014
 *      Author: mike_gresens
 */

#pragma once

#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>
#include <boost/function.hpp>

namespace rt {
namespace rendering {

typedef boost::gil::rgb32f_image_t image_t;

typedef image_t::coord_t coord_t;
typedef image_t::value_type pixel_t;
typedef image_t::view_t view_t;
typedef image_t::const_view_t const_view_t;

typedef boost::function<void (const_view_t)> image_writer_t;

image_writer_t
make_writer(const std::string& output);

}
}
