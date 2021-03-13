/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020-2021 Skyler Burwell
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifndef __VGDE_VEC_H__
#define __VGDE_VEC_H__

#include "config.h"

#include <iostream>

template<typename T>
class vec2 {
public:
	inline vec2() :
		x(0),
		y(0)
	{
		//
	}

	inline explicit vec2(T _x) :
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

	static const vec2<T> Zero;

	T x;
	T y;
};

template<typename T>
inline vec2<T> operator -(const vec2<T> &rhs) {
	return vec2<T>(-rhs.x, -rhs.y);
}

template<typename T>
inline vec2<T> operator +=(vec2<T> &lhs, const vec2<T> &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;

	return lhs;
}

template<typename T>
inline vec2<T> operator -=(vec2<T> &lhs, const vec2<T> &rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;

	return lhs;
}

template<typename T>
inline vec2<T> operator +(const vec2<T> &lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<typename T>
inline vec2<T> operator +(const vec2<T> &lhs, T rhs) {
	return vec2<T>(lhs.x + rhs, lhs.y + rhs);
}

template<typename T>
inline vec2<T> operator +(T lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs + rhs.x, lhs + rhs.y);
}

template<typename T>
inline vec2<T> operator -(const vec2<T> &lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template<typename T>
inline vec2<T> operator -(const vec2<T> &lhs, T rhs) {
	return vec2<T>(lhs.x - rhs, lhs.y - rhs);
}

template<typename T>
inline vec2<T> operator -(T lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs - rhs.x, lhs - rhs.y);
}

template<typename T>
inline vec2<T> operator *(const vec2<T> &lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
}

template<typename T>
inline vec2<T> operator *(const vec2<T> &lhs, T rhs) {
	return vec2<T>(lhs.x * rhs, lhs.y * rhs);
}

template<typename T>
inline vec2<T> operator *(T lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs * rhs.x, lhs * rhs.y);
}

template<typename T>
inline vec2<T> operator *=(vec2<T> &lhs, const vec2<T> &rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;

	return lhs;
}

template<typename T>
inline vec2<T> operator /(const vec2<T> &lhs, const vec2<T> &rhs) {
	return vec2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
}

template<typename T>
inline vec2<T> operator /(const vec2<T> &lhs, T rhs) {
	return vec2<T>(lhs.x / rhs, lhs.y / rhs);
}

template<typename T>
inline vec2<T> operator /=(vec2<T> &lhs, const vec2<T> &rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;

	return lhs;
}

template<typename T>
inline vec2<T> operator /=(vec2<T> &lhs, T rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;

	return lhs;
}

template<typename T>
inline bool operator ==(const vec2<T> &lhs, const vec2<T> &rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

template<typename T>
inline bool operator !=(const vec2<T> &lhs, const vec2<T> &rhs) {
	return !(lhs == rhs);
}

template <typename T>
inline std::ostream &operator <<(std::ostream &os, vec2<T> &vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    
    return os;
}

template <typename T>
inline std::ostream &operator <<(std::ostream &os, const vec2<T> &vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    
    return os;
}

template<typename T>
const vec2<T> vec2<T>::Zero(0, 0);

typedef vec2<int>    vec2i;
typedef vec2<uint32> vec2u;
typedef vec2<float>  vec2f;

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

	inline explicit vec3(T _x) :
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
	explicit inline vec3(const vec3<U> &v) :
		x(staticCast(T, v.x)),
		y(staticCast(T, v.y)),
		z(staticCast(T, v.z))
	{
		//
	}

	static const vec3<T> Zero;

	T x;
	T y;
	T z;
};

template<typename T>
inline vec3<T> operator -(const vec3<T> &rhs) {
	return vec3<T>(-rhs.x, -rhs.y, -rhs.z);
}

template<typename T>
inline vec3<T> operator +=(vec3<T> &lhs, const vec3<T> &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;

	return lhs;
}

template<typename T>
inline vec3<T> operator -=(vec3<T> &lhs, const vec3<T> &rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;

	return lhs;
}

template<typename T>
inline vec3<T> operator +(const vec3<T> &lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template<typename T>
inline vec3<T> operator +(const vec3<T> &lhs, T rhs) {
	return vec3<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
}

template<typename T>
inline vec3<T> operator +(T lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
}

template<typename T>
inline vec3<T> operator -(const vec3<T> &lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template<typename T>
inline vec3<T> operator -(const vec3<T> &lhs, T rhs) {
	return vec3<T>(lhs.x - rhs, lhs.y - rhs, lhs.z + rhs);
}

template<typename T>
inline vec3<T> operator -(T lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs - rhs.x, lhs - rhs.y, lhs + rhs.z);
}

template<typename T>
inline vec3<T> operator *(const vec3<T> &lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

template<typename T>
inline vec3<T> operator *(const vec3<T> &lhs, T rhs) {
	return vec3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z + rhs);
}

template<typename T>
inline vec3<T> operator *(T lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs * rhs.x, lhs * rhs.y, lhs + rhs.z);
}

template<typename T>
inline vec3<T> operator *=(vec3<T> &lhs, const vec3<T> &rhs) {
	lhs.x *= rhs.x;
	lhs.y *= rhs.y;
	lhs.z *= rhs.z;

	return lhs;
}

template<typename T>
inline vec3<T> operator /(const vec3<T> &lhs, const vec3<T> &rhs) {
	return vec3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

template<typename T>
inline vec3<T> operator /(const vec3<T> &lhs, T rhs) {
	return vec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

template<typename T>
inline vec3<T> operator /=(vec3<T> &lhs, const vec3<T> &rhs) {
	lhs.x /= rhs.x;
	lhs.y /= rhs.y;
	lhs.z /= rhs.z;

	return lhs;
}

template<typename T>
inline vec3<T> operator /=(vec3<T> &lhs, T rhs) {
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;

	return lhs;
}

template<typename T>
inline bool operator ==(const vec3<T> &lhs, const vec3<T> &rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.x == rhs.z);
}

template<typename T>
inline bool operator !=(const vec3<T> &lhs, const vec3<T> &rhs) {
	return !(lhs == rhs);
}

template <typename T>
inline std::ostream &operator <<(std::ostream &os, vec3<T> &vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    
    return os;
}

template <typename T>
inline std::ostream &operator <<(std::ostream &os, const vec3<T> &vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    
    return os;
}

template<typename T>
const vec3<T> vec3<T>::Zero(0, 0, 0);

typedef vec3<int>    vec3i;
typedef vec3<uint32> vec3u;
typedef vec3<float>  vec3f;

#endif
