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
