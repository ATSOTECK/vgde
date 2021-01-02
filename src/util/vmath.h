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

#ifndef __VGDE_VMATH_H__
#define __VGDE_VMATH_H__

#include "config.h"
#include "vec.h"

#include <cstdarg>
#include <cstdlib>
#include <vector>

#define V_PI  3.14159265358979323846
#define V_TAU 6.28318530718

void randomInit();
void randomSetSeed(int seed);
void randomSetRandomSeed();
int randomGetSeed();

float random(int n = 100);
float randomRange(int n, int n1);

int randomi(int n = 100);
int randomRangei(int n, int n1);

vec2f randomPosition(float x, float y);

template<typename T>
T vclamp(T val, T min, T max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}

	return val;
}

namespace {
typedef int (*__clampi_type)(int, int, int);
typedef float (*__clampf_type)(float, float, float);
}

__clampi_type const clampi = &vclamp<int>;
__clampf_type const clampf = &vclamp<float>;

//Shortcut for Color.
uint8 clampc(uint8 val);

float degToRad(float val);
float radToDeg(float val);

/*
template<typename T>
T choose(int count, ...) {
	std::vector<T> retVec;
	va_list args;

	va_start(args, count);
	for (int i = 0; i < count; ++i) {
		T t = va_arg(args, T);
		retVec.push_back(t);
	}
	va_end(args);

	return retVec.at(randomi(count));
}
*/

#endif
