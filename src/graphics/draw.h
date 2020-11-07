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

#ifndef __VGDE_DRAW_H__
#define __VGDE_DRAW_H__

#include "color.h"
#include "config.h"
#include "font.h"
#include "rect.h"
#include "sprite.h"
#include "texture.h"
#include "vec.h"
#include "util/vstring.h"

#define VERT_SIZE 6

class Vertex {
public:
	Vertex(float x, float y, float r, float g, float b, float a);

	float x;
	float y;

	float r;
	float g;
	float b;
	float a;
};

void drawInit();

glm::mat4 drawGetProjection();
void drawSetProjection(float left, float right, float bottom, float top, float zNear, float zFar);
void drawSetProjection(const glm::mat4 &proj);

Color drawGetColor();
void drawSetColor(const Color &c);

Color drawGetClearColor();
void drawSetClearColor(const Color &c);

uint8 drawGetAlpha();
void drawSetAlpha(uint8 a);

int drawGetFontSize();
void drawSetFontSize(int size);

//The vertex is assumed to be x, y, r, g, b, a.
void drawVerts(float *verts, int count, bool outline);

void drawLine(float x, float y, float x1, float y1);
void drawLine(const vec2f &pos, const vec2f &pos1);

void drawRectangle(float x, float y, float w, float h, bool outline = false);
void drawRectangle(const vec2f &pos, const vec2f &size, bool outline  = false);
void drawRectangle(const rectf &rect, bool outline = false);

void drawCircle(float x, float y, float r, int sides = 20, bool outline = true);
void drawArc(float x, float y, float r, float a, float a1, int sides = 20, bool radians = false);
void drawSlice(float x, float y, float r, float a, float a1, int sides = 20, bool outline = true, bool radians = false);

float drawText(const String &txt, float x, float y, float scale, const Color &color, Font *font);
float drawText(const String &txt, float x, float y, float scale, const Color &color);

#endif
