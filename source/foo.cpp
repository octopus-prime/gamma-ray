/*
 * foo.cpp
 *
 *  Created on: 25.02.2015
 *      Author: mgresens
 */

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <vector>

// --------------------------------------------------

typedef int vector_t;
typedef int ray_t;

class surface
{
public:
	typedef int hit_t;
	typedef std::vector<hit_t> hits_t;

protected:
	virtual ~surface() noexcept = default;

public:
	virtual hits_t::iterator hit(const ray_t& r, hits_t::iterator i) const noexcept = 0;
};

typedef boost::shared_ptr<surface> surface_t;

class sphere
:
	public virtual surface
{
public:
	virtual hits_t::iterator hit(const ray_t& r, hits_t::iterator i) const noexcept override
	{
		return i;
	}
};

typedef boost::shared_ptr<sphere> sphere_t;

// --------------------------------------------------

template <typename T>
using basic_procedural_t = boost::function<T (vector_t)>;

typedef basic_procedural_t<double> noise_procedural_t;
typedef basic_procedural_t<vector_t> vector_procedural_t;
typedef basic_procedural_t<vector_t> color_procedural_t;

class texture
{
public:
	vector_t pigment(const vector_t& point) const noexcept
	{
		return _pigment(point);
	}

	vector_t bump(const vector_t& point) const noexcept
	{
		return _bump(point);
	}

	double ambient(const vector_t& point) const noexcept
	{
		return _ambient(point);
	}

private:
	color_procedural_t _pigment;
	vector_procedural_t _bump;
	noise_procedural_t _ambient;
};

typedef boost::shared_ptr<texture> texture_t;

// --------------------------------------------------

class object;
typedef boost::shared_ptr<object> object_t;

class object
{
public:
	typedef std::pair<int, const object*> hit_t;
	typedef std::vector<hit_t> hits_t;

protected:
	virtual ~object() noexcept = default;

public:
	virtual hits_t::iterator hit(const ray_t& r, hits_t::iterator i) const noexcept = 0;
	virtual texture_t texture() const noexcept = 0;
};

class primitive
:
	public virtual object
{
public:
	primitive(const surface_t& surface, const texture_t& texture) noexcept
	:
		_surface(surface),
		_texture(texture)
	{
	}

	virtual hits_t::iterator hit(const ray_t& r, hits_t::iterator i) const noexcept override
	{
		// TODO: how to delegate the space for surface hits by using the given space for the object hits.
		// TODO: iterator adapter ?!
		// transform
		surface::hits_t h;
		const auto e = _surface->hit(r, h.begin());
		// transform
		return std::transform(h.begin(), e, i, [this](const surface::hit_t& h){return std::make_pair(h, this);});
	}

	virtual texture_t texture() const noexcept override
	{
		return _texture;
	}

private:
	surface_t _surface;
	texture_t _texture;
};

typedef boost::shared_ptr<primitive> primitive_t;

class csg
:
	public virtual object
{
public:
	virtual texture_t texture() const noexcept override
	{
		throw std::domain_error("foo");
	}

protected:
	csg(const object_t& object1, const object_t& object2) noexcept
	:
		_object1(object1),
		_object2(object2)
	{
	}

	object_t _object1;
	object_t _object2;
};

typedef boost::shared_ptr<csg> csg_t;

class intersection
:
	public csg
{
public:
	intersection(const object_t& object1, const object_t& object2) noexcept
	:
		csg(object1, object2)
	{
	}

	virtual hits_t::iterator hit(const ray_t& r, hits_t::iterator i) const noexcept override
	{
		// TODO: how to know the primitive objects here?!
		const auto e1 = _object1->hit(r, i);
		const auto e2 = _object2->hit(r, e1);
		return e2;
	}
};

typedef boost::shared_ptr<intersection> intersection_t;

void test()
{
	const object_t object = boost::make_shared<intersection>
	(
		boost::make_shared<primitive>
		(
			boost::make_shared<sphere>(),
			boost::make_shared<texture>()
		),
		boost::make_shared<primitive>
		(
			boost::make_shared<sphere>(),
			boost::make_shared<texture>()
		)
	);

	object::hits_t hits(100);
	const object::hits_t::iterator begin = hits.begin();
	const object::hits_t::iterator end = object->hit(ray_t(), begin);

	std::for_each
	(
		begin, end,
		[](const object::hit_t& hit)
		{
			const texture_t texture = hit.second->texture();
			texture->pigment(0);
			texture->bump(0);
			texture->ambient(0);
		}
	);
}
