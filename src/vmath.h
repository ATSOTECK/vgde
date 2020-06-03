#ifndef __VGDE_VMATH_H__
#define __VGDE_VMATH_H__

#include <cstdarg>
#include <cstdlib>
#include <vector>

void randomInit();
void randomSetSeed(int seed);
void randomSetRandomSeed();
int randomGetSeed();

float random(int n = 100);
float randomRange(int n, int n1);

int randomi(int n = 100);
int randomRangei(int n, int n1);

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
