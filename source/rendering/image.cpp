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
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>

namespace fs = boost::filesystem;
namespace gil = boost::gil;

namespace rt {
namespace rendering {

image_writer_t
make_writer(const std::string& output)
{
	const fs::path file(output);
	fs::create_directories(file.parent_path());

	const boost::unordered_map<fs::path, image_writer_t> writers = boost::assign::map_list_of<fs::path, image_writer_t>
	(
		".jpg", [file](const const_view_t& view)
		{
			gil::jpeg_write_view
			(
				file.string(),
				gil::color_converted_view<gil::rgb8_pixel_t>(view)
			);
		}
	)
	(
		".png", [file](const const_view_t& view)
		{
			gil::png_write_view
			(
				file.string(),
				gil::color_converted_view<gil::rgb16_pixel_t>(view)
			);
		}
	)
	(
		".tif", [file](const const_view_t& view)
		{
			gil::tiff_write_view
			(
				file.string(),
				view
			);
		}
	);

	const fs::path extension = file.extension();

	try
	{
		return writers.at(extension);
	}
	catch (const std::out_of_range& exception)
	{
		throw std::runtime_error("Extension '" + extension.string() + "' not valid. Use '.jpg', '.png' or '.tif'.");
	}
}

}
}
