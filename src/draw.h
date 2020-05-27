#ifndef __VGDE_DRAW_H__
#define __VGDE_DRAW_H__

#include "config.h"
#include "sprite.h"
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

	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
};

Color drawGetColor();
void drawSetColor(const Color &c);

Color drawGetClearColor();
void drawSetClearColor(const Color &c);

uint8 drawGetAlpha();
void drawSetAlpa(uint8 a);

void drawLine(float x, float y, float x1, float y1);
void drawLine(const vec2f &pos, const vec2f &pos1);

void drawSprite(const Sprite &spr);

#endif
