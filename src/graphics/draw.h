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

void drawText(const String &txt, float x, float y, float scale, const Color &color, Font *font);
void drawText(const String &txt, float x, float y, float scale, const Color &color);

#endif
