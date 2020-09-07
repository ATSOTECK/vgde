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

vec2f randomPosition(float x, float y) {
    return {random((int)x), random((int)y)};
}

uint8 clampc(uint8 val) {
	return vclamp<uint8>(val, 0, 255);
}

float degToRad(float val) {
	return 0.01745329251994329576923690768489f * val;
}

float radToDeg(float val) {
	return 57.295779513082320876798154814105f * val;
}
