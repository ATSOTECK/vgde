#include "draw.h"

#include "vgde.h"

namespace {
VGDE *vgde = VGDE::instance();
}

const Color Color::Black(0, 0, 0);
const Color Color::Black100(0, 0, 0, 100);
const Color Color::Black150(0, 0, 0, 150);
const Color Color::Black200(0, 0, 0, 200);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::LightBlue(72, 118, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Turquoise(64, 244, 208);
const Color Color::Transparent(0, 0, 0, 0);
const Color Color::None(0, 0, 0, 0);

Color::Color(uint8 red, uint8 green, uint8 blue, uint8 alpha) :
	r(red),
	g(green),
	b(blue),
	a(alpha)
{
	//
}

Color drawGetColor() {
	GLfloat glc[4];
	glGetFloatv(GL_CURRENT_COLOR, glc);

	Color c(glc[0] * 255, glc[1] * 255, glc[2] * 255, glc[3] * 255);

	return c;
}

void drawSetColor(const Color &c) {
	GLfloat r = c.r / 255.f;
	GLfloat g = c.g / 255.f;
	GLfloat b = c.b / 255.f;
	GLfloat a = c.a / 255.f;

	glColor4f(r, g, b, a);
}

Color drawGetClearColor() {
	GLfloat glc[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, glc);

	Color c(glc[0] * 255, glc[1] * 255, glc[2] * 255, glc[3] * 255);

	return c;
}

void drawSetClearColor(const Color &c) {
	GLfloat r = c.r / 255.f;
	GLfloat g = c.g / 255.f;
	GLfloat b = c.b / 255.f;
	GLfloat a = c.a / 255.f;

	glClearColor(r, g, b, a);
}

uint8 drawGetAlpha() {
	Color c = drawGetColor();

	return c.a;
}

void drawSetAlpa(uint8 a) {
	Color c = drawGetColor();
	c.a = a;

	drawSetColor(c);
}

void drawLine(float x, float y, float x1, float y1) {
	float coords[] = {x, y, x1, y1};
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, (const GLvoid *)coords);
	glDrawArrays(GL_LINES, 0, (GLsizei)2); // opengl will close the polygon for us
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LINE_SMOOTH);
}

void drawLine(const vec2f &pos, const vec2f &pos1) {
	drawLine(pos.x, pos.y, pos1.x, pos1.y);
}
