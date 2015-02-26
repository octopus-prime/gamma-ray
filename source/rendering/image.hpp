/*
 * image.hpp
 *
 *  Created on: 24.05.2014
 *      Author: mike_gresens
 */

#pragma once

#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>

namespace rt {
namespace rendering {

typedef boost::gil::rgb32f_pixel_t pixel_t;
typedef boost::gil::rgb32f_image_t image_t;
typedef boost::gil::rgb32f_view_t view_t;
typedef image_t::coord_t coord_t;

typedef std::function<void (view_t, std::string)> image_writer_t;

image_writer_t make_writer(const std::string& extension);

}
}
