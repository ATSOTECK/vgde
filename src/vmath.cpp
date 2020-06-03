#include "vmath.h"

#include "config.h"

#include <math.h>
#include <time.h>

namespace {
	int _seed;
}

void randomInit() {
	srand(time(null));
	randomSetRandomSeed();
}

void randomSetSeed(int seed) {
	_seed = seed;
	srand(_seed);
}

void randomSetRandomSeed() {
	int seed = rand() % RAND_MAX;
	randomSetSeed(seed);
}

int randomGetSeed() {
	return _seed;
}

float random(int n) {
	return staticCastf(rand()) / staticCastf(RAND_MAX / n);
}

float randomRange(int n, int n1) {
	float r = staticCastf(rand()) / staticCastf(RAND_MAX / (n1 - n));
	return r + n;
}

int randomi(int n) {
	return rand() % n;
}

int randomRangei(int n, int n1) {
	int r = rand() % (n1 - n);
	return r + n;
}

/*
template<class T>
T choose(int count, T t, ...) {
	std::vector<T> retVec;
	va_list args;

	va_start(args, t);
	for (int i = 0; i < count; ++i) {
		retVec.push_back(va_arg(args, T));
	}
	va_end(args);

	return retVec.at(randomi(count));
}
*/
