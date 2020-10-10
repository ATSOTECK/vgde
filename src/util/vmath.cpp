/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020 Skyler Burwell
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
