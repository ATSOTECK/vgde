#ifndef __VGDE_DRAW_H__
#define __VGDE_DRAW_H__

#include "config.h"
#include "rect.h"
#include "sprite.h"
#include "texture.h"
#include "vec.h"

class Color {
public:
	Color(uint8 red = 0, uint8 green = 0, uint8 blue = 0, uint8 alpha = 255);

	static const Color Black;
	static const Color Black100;
	static const Color Black150;
	static const Color Black200;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color LightBlue;
	static const Color Yellow;
	static const Color Magenta;
	static const Color Cyan;
	static const Color Turquoise;
	static const Color Transparent;
	static const Color None;

	Color &operator=(const Color &other);

	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;

	float glR;
	float glG;
	float glB;
	float glA;
};

bool operator ==(const Color &lhs, const Color &rhs);
bool operator !=(const Color &lhs, const Color &rhs);
Color operator +(const Color &lhs, const Color &rhs);
Color operator -(const Color &lhs, const Color &rhs);
Color operator *(const Color &lhs, const Color &rhs);
Color &operator +=(Color &lhs, const Color &rhs);
Color &operator -=(Color &lhs, const Color &rhs);
Color &operator *=(Color &lhs, const Color &rhs);

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

void drawSprite(const Sprite &spr);

#endif
