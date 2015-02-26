/*
 * foo.cpp
 *
 *  Created on: 25.02.2015
 *      Author: mgresens
 */

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <vector>

class surface
{
public:
	typedef int hit_t;
	typedef std::vector<hit_t> hits_t;

protected:
	virtual ~surface() noexcept = default;

public:
	virtual hits_t::iterator hit(int r, hits_t::iterator i) const noexcept = 0;
};

typedef boost::shared_ptr<surface> surface_t;

class sphere
:
	public surface
{
public:
	virtual hits_t::iterator hit(int r, hits_t::iterator i) const noexcept override
	{
		return i;
	}
};

typedef boost::shared_ptr<sphere> sphere_t;

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
	virtual hits_t::iterator hit(int r, hits_t::iterator i) const noexcept = 0;
};

class primitive
:
	public virtual object
{
public:
	primitive(const surface_t& surface) noexcept
	:
		_surface(surface)
	{
	}

	virtual hits_t::iterator hit(int r, hits_t::iterator i) const noexcept override
	{
		// TODO: how to delegate the space for surface hits by using the given space for the object hits.
		// TODO: iterator adapter ?!
		// transform
		surface::hits_t h;
		const auto e = _surface->hit(r, h.begin());
		// transform
		return std::transform(h.begin(), e, i, [this](const surface::hit_t& h){return std::make_pair(h, this);});
	}

private:
	surface_t _surface;
};

typedef boost::shared_ptr<primitive> primitive_t;

class csg
:
	public virtual object
{
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

	virtual hits_t::iterator hit(int r, hits_t::iterator i) const noexcept override
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
	const object_t o = boost::make_shared<intersection>
	(
		boost::make_shared<primitive>(boost::make_shared<sphere>()),
		boost::make_shared<primitive>(boost::make_shared<sphere>())
	);
	object::hits_t hits(100);
	object::hits_t::iterator end = o->hit(0, hits.begin());
}
