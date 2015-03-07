/*
 * make.cpp
 *
 *  Created on: 22.02.2015
 *      Author: mike_gresens
 */

#include <scene/object/surface/primitive/text/make.hpp>
#include <scene/object/surface/primitive/text/model.hpp>
#include <math/transformation.hpp>
#include <scene/object/surface/primitive/instance.hpp>
#include <boost/log/trivial.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BBOX_H
#include FT_ERRORS_H

namespace rt {
namespace scene {
namespace object {
namespace surface {
namespace primitive {

extern template class instance<text::model>;

namespace text {

// TODO: separate file
class Decomposer
{
private:
	FT_Outline_Funcs		_functions;
	std::vector<contour_t>	_contours;
	vector2_t				_fromPoint;
	float					_advanceX;
	float					_advanceY;

protected:
	static int
	moveToFunc(const FT_Vector* to, void* user)
	{
		Decomposer* decomposer = reinterpret_cast<Decomposer*>(user);
		decomposer->_fromPoint = makePoint(*to, decomposer->_advanceX, decomposer->_advanceY);
		return 0;
	}

	static int
	lineToFunc(const FT_Vector* to, void* user)
	{
		Decomposer* decomposer = reinterpret_cast<Decomposer*>(user);
		const vector2_t toPoint = makePoint(*to, decomposer->_advanceX, decomposer->_advanceY);
		const bezier1_t bezier {{decomposer->_fromPoint, toPoint}};
		decomposer->_contours.push_back(std::make_tuple(to_polynomial(bezier), to_polynomial(differentiate(bezier))));
		decomposer->_fromPoint = toPoint;
		return 0;
	}

	static int
	conicToFunc(const FT_Vector* c1, const FT_Vector* to, void* user)
	{
		Decomposer* decomposer = reinterpret_cast<Decomposer*>(user);
		const vector2_t c1Point = makePoint(*c1, decomposer->_advanceX, decomposer->_advanceY);
		const vector2_t toPoint = makePoint(*to, decomposer->_advanceX, decomposer->_advanceY);
		const bezier2_t bezier {{decomposer->_fromPoint, c1Point, toPoint}};
		decomposer->_contours.push_back(std::make_tuple(to_polynomial(bezier), to_polynomial(differentiate(bezier))));
		decomposer->_fromPoint = toPoint;
		return 0;
	}

	static int
	cubicToFunc(const FT_Vector* c1, const FT_Vector* c2, const FT_Vector* to, void* user)
	{
		Decomposer* decomposer = reinterpret_cast<Decomposer*>(user);
		const vector2_t c1Point = makePoint(*c1, decomposer->_advanceX, decomposer->_advanceY);
		const vector2_t c2Point = makePoint(*c2, decomposer->_advanceX, decomposer->_advanceY);
		const vector2_t toPoint = makePoint(*to, decomposer->_advanceX, decomposer->_advanceY);
		const bezier3_t bezier {{decomposer->_fromPoint, c1Point, c2Point, toPoint}};
		decomposer->_contours.push_back(std::make_tuple(to_polynomial(bezier), to_polynomial(differentiate(bezier))));
		decomposer->_fromPoint = toPoint;
		return 0;
	}

	static vector2_t
	makePoint(const FT_Vector& point, const float advanceX, const float advanceY)
	{
		return {{(advanceX + point.x) / 1000.0f, (advanceY + point.y) / 1000.0f}};
	}

public:
	Decomposer(const float advanceX, const float advanceY)
	:
		_functions({moveToFunc, lineToFunc, conicToFunc, cubicToFunc}),
		_contours(),
		_fromPoint{{0.0, 0.0}},
		_advanceX(advanceX),
		_advanceY(advanceY)
	{
	}

	std::vector<contour_t>
	operator()(FT_Outline* outline)
	{
		const FT_Error error = FT_Outline_Decompose(outline, &_functions, this);
		if (error)
			throw std::runtime_error("Can't decompose outline. Code = " + std::to_string(error));
		return _contours;
	}
};

typedef std::shared_ptr<FT_LibraryRec_> Library;
typedef std::shared_ptr<FT_FaceRec_> Face;
typedef std::shared_ptr<FT_GlyphRec> Glyph;

static Library
init_library()
{
	FT_Library handle = nullptr;
	const FT_Error error = FT_Init_FreeType(&handle);
	const Library library(handle, FT_Done_FreeType);
	if (error)
		throw std::runtime_error("Can't initiate freetype library. Code = " + std::to_string(error));
	return library;
}

static Face
load_face(const Library& library, const std::string& font)
{
	FT_Face handle = nullptr;
	const FT_Error error = FT_New_Face(library.get(), font.data(), 0, &handle);
	const Face face(handle, FT_Done_Face);
	if (error)
		throw std::runtime_error("Can't load freetype face '" + font + "'. Code = " + std::to_string(error));
	return face;
}

static Glyph
get_glyph(const Face& face, const char32_t ch, const std::size_t sizeX, const std::size_t sizeY, const std::size_t resolutionX, const std::size_t resolutionY)//, const double advanceX)
{
	FT_Error error = FT_Set_Char_Size(face.get(), sizeX << 6, sizeX << 6, resolutionX, resolutionY);
	if (error)
		throw std::runtime_error("Can't set glyph size. Code = " + std::to_string(error));

	const FT_UInt index = FT_Get_Char_Index(face.get(), ch);

	error = FT_Load_Glyph(face.get(), index, FT_LOAD_DEFAULT);
	if (error)
		throw std::runtime_error("Can't load glyph. Code = " + std::to_string(error));

	if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
		throw std::runtime_error("Glyph is not outlined.");

    FT_Glyph handle = nullptr;
    error = FT_Get_Glyph(face->glyph, &handle);
	const Glyph glyph(handle, FT_Done_Glyph);
	if (error)
		throw std::runtime_error("Can't get glyph. Code = " + std::to_string(error));
	return glyph;
}


boost::tuple<surface::instance_t, box_t>
make(const description_t& description)
{
	BOOST_LOG_TRIVIAL(debug) << "Make surface text";

	const Library library = init_library();
	const Face face = load_face(library, description->font);

	float xadvance = 0.0f;
	float yadvance = 0.0f;
	glyphs_t glyphs;
	rtree_t rtree;

	for (std::size_t i = 0, n = 0; i < description->text.size(); ++i)
	{
		const char32_t ch = description->text[i];

		if (ch != '?')
		{
		const Glyph glyph = get_glyph(face, ch, 25, 25, 300, 300);//, xadvance);
		const FT_Outline* outline = &reinterpret_cast<FT_OutlineGlyph>(glyph.get())->outline;

		FT_BBox bbox;
		const FT_Error error = FT_Outline_Get_BBox(const_cast<FT_Outline*>(outline), &bbox);
		if (error)
			throw std::runtime_error("Can't get bbox. Code = " + std::to_string(error));

		const box_t box
		(
			{{(bbox.xMin + xadvance) / 1e3f, (bbox.yMin + yadvance) / 1e3f, 0}},
			{{(bbox.xMax + xadvance) / 1e3f, (bbox.yMax + yadvance) / 1e3f, description->extrusion}}
		);

		Decomposer decompose(xadvance, yadvance);
		contours_t contours = decompose(const_cast<FT_Outline*>(outline));

		xadvance += face->glyph->metrics.horiAdvance;

		glyphs.emplace_back(std::move(contours), description->extrusion);
		rtree.insert(value_t(box, n));
		n++;
		}
		else
		{
			xadvance = 0;
			yadvance -= face->glyph->metrics.vertAdvance;
		}
	}

	const box_t box = transform
	(
		description->transformation,
		box_t// TODO: puke =>
		(
			{{
				geo::get<X>(rtree.bounds().min_corner()),
				geo::get<Y>(rtree.bounds().min_corner()),
				geo::get<Z>(rtree.bounds().min_corner())
			}},
			{{
				geo::get<X>(rtree.bounds().max_corner()),
				geo::get<Y>(rtree.bounds().max_corner()),
				geo::get<Z>(rtree.bounds().max_corner())
			}}
		)
	);

	BOOST_LOG_TRIVIAL(trace) << "Box: min = " << box.min_corner() << ", max = " << box.max_corner() << std::endl;

	return boost::make_tuple
	(
		primitive::make<model>
		(
			description->transformation,
			std::move(glyphs),
			std::move(rtree),
			description->extrusion
		),
		box
	);
}

}
}
}
}
}
}
