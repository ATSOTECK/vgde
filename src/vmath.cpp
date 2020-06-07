#include "vmath.h"

#include "config.h"

#define _USE_MATH_DEFINES
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

uint8 clampc(uint8 val) {
	return vclamp<uint8>(val, 0, 255);
}

float degToRad(float val) {
	return (M_PI / 180) * val;
}

float radToDeg(float val) {
	return (float)(180 / M_PI) * val;
}
