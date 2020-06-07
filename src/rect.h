#ifndef __VGDE_RECT_H__
#define __VGDE_RECT_H__

#include "config.h"
#include "vec.h"

template<typename T>
class vrect {
public:
	inline vrect() :
		x(0),
		y(0),
		width(0),
		height(0)
	{
		//
	}

	inline vrect(T _x, T _y, T w, T h) :
		x(_x),
		y(_y),
		width(w),
		height(h) 
	{
		//
	}

	inline vrect(const vec2<T> &pos, const vec2<T> &size) :
		x(pos.x),
		y(pos.y),
		width(size.x),
		height(size.y) 
	{
		//
	}

	template<typename U>
	explicit inline vrect(const vrect<U> &r) :
		x(staticCast(T, r.x)),
		y(staticCast(T, r.y)),
		width(staticCast(T, r.width)),
		height(staticCast(T, r.height)) 
	{
		//
	}

	inline vec2<T> getPosition() const {
		vec2<T> pos;
		pos.x = x;
		pos.y = y;

		return pos;
	}

	inline vec2<T> getSize() const {
		vec2<T> size;
		size.x = width;
		size.y = height;

		return size;
	}

	T x;
	T y;
	T width;
	T height;
};

template<typename T>
inline bool operator ==(const vrect<T> &lhs, const vrect<T> &rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height);
}

template<typename T>
inline bool operator !=(const vrect<T> &lhs, const vrect<T> &rhs) {
	return !(lhs == rhs);
}

typedef vrect<int>    rect;
typedef vrect<uint32> rectu;
typedef vrect<float>  rectf;

#endif
