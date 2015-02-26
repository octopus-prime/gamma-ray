/*
 * image.cpp
 *
 *  Created on: 24.05.2014
 *      Author: mike_gresens
 */

// Hack for Mac :(
// Should be part of boost::gil !!
#if defined(__APPLE__) && defined(__MACH__)
#define png_infopp_NULL nullptr
#define int_p_NULL nullptr
#define png_bytep_NULL nullptr
#endif

#include <rendering/image.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>
#include <unordered_map>

namespace rt {
namespace rendering {

image_writer_t
make_writer(const std::string& extension)
{
	static const std::unordered_map<std::string, image_writer_t> WRITERS
	{
		{
			".jpg",
			[](const view_t& view, const std::string& file)
			{
				boost::gil::jpeg_write_view
				(
					file,
					boost::gil::color_converted_view<boost::gil::rgb8_pixel_t>(view)
				);
			}
		},
		{
			".png",
			[](const view_t& view, const std::string& file)
			{
				boost::gil::png_write_view
				(
					file,
					boost::gil::color_converted_view<boost::gil::rgb16_pixel_t>(view)
				);
			}
		},
		{
			".tif",
			[](const view_t& view, const std::string& file)
			{
				boost::gil::tiff_write_view
				(
					file,
					view
				);
			}
		}
	};

	try
	{
		return WRITERS.at(extension);
	}
	catch (const std::out_of_range& exception)
	{
		throw std::runtime_error("Extension '" + extension + "' not valid. Use '.jpg', '.png' or '.tif'.");
	}
}

}
}
