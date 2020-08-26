#ifndef __VGDE_DRAW_H__
#define __VGDE_DRAW_H__

#include "color.h"
#include "config.h"
#include "rect.h"
#include "sprite.h"
#include "texture.h"
#include "vec.h"

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

Color drawGetColor();
void drawSetColor(const Color &c);

Color drawGetClearColor();
void drawSetClearColor(const Color &c);

uint8 drawGetAlpha();
void drawSetAlpa(uint8 a);

void drawLine(float x, float y, float x1, float y1);
void drawLine(const vec2f &pos, const vec2f &pos1);

void drawRectangle(float x, float y, float w, float h, bool outline = false);
void drawRectangle(const vec2f &pos, const vec2f &size, bool outline  = false);
void drawRectangle(const rectf &rect, bool outline = false);

void drawText(const std::string &txt, float x, float y, float scale, const Color &color);

#endif
