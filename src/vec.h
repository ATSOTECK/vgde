#ifndef __VGDE_VEC_H__
#define __VGDE_VEC_H__

#include "config.h"

template<typename T>
class vec2 {
public:
	inline vec2() :
		x(0),
		y(0)
	{
		//
	}

	inline vec2(T _x) :
		x(_x),
		y(_x) 
	{
		//
	}

	inline vec2(T _x, T _y) :
		x(_x),
		y(_y)
	{
		//
	}

	template<typename U>
	explicit inline vec2(const vec2<U> &v) :
		x(staticCast(T, v.x)),
		y(staticCast(T, v.y)) 
	{
		//
	}

	T x;
	T y;
};

template<typename T>
class vec3 {
public:
	inline vec3() :
		x(0),
		y(0),
		z(0)
	{
		//
	}

	inline vec3(T _x) :
		x(_x),
		y(_x),
		z(_x)
	{
		//
	}

	inline vec3(T _x, T _y, T _z) :
		x(_x),
		y(_y),
		z(_z)
	{
		//
	}

	template<typename U>
	explicit inline vec3(const vec2<U> &v) :
		x(staticCast(T, v.x)),
		y(staticCast(T, v.y)),
		z(staticCast(T, v.z))
	{
		//
	}

	T x;
	T y;
	T z;
};

#endif
