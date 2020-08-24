#include "draw.h"

#include "shader.h"
#include "vgde.h"
#include "util/vmath.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace {
VGDE *_vgde = VGDE::instance();

Shader *_shader;

glm::mat4 _projection;
GLuint _vbo;
GLuint _ibo;
Color _color;

static const std::string defaultVertexShader = 
"#version 330 core\n"
"layout (location = 0) in vec4 pos;\n"
"layout (location = 1) in vec4 inColor;\n"
"uniform mat4 model;\n"
"uniform mat4 projection;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * vec4(pos.xy, 0, 1.0);\n"
"   color = inColor;"
"}";

static const std::string defaultFragmentShader = 
"#version 330 core\n"
"out vec4 outColor;\n"
"in vec4 color;"
"void main()\n"
"{\n"
"   outColor = color;\n"
"}";
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
	glR = r / 255.f;
	glG = g / 255.f;
	glB = b / 255.f;
	glA = a / 255.f;
}

Color &Color::operator= (const Color &other) {
	if (this == &other) {
		return *this;
	}

	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;

	glR = other.glR;
	glG = other.glG;
	glB = other.glB;
	glA = other.glA;

	return *this;
}

bool operator ==(const Color &lhs, const Color &rhs) {
	return (lhs.r == rhs.r) &&
		   (lhs.g == rhs.g) &&
		   (lhs.b == rhs.b) &&
		   (lhs.a == rhs.a);
}

bool operator !=(const Color &lhs, const Color &rhs) {
	return !(lhs == rhs);
}

Color operator +(const Color &lhs, const Color &rhs) {
	return Color(clampc(lhs.r + rhs.r),
				 clampc(lhs.g + rhs.g),
				 clampc(lhs.b + rhs.b),
				 clampc(lhs.a + rhs.a));
}

Color operator -(const Color &lhs, const Color &rhs) {
	return Color(clampc(lhs.r - rhs.r),
				 clampc(lhs.g - rhs.g),
				 clampc(lhs.b - rhs.b),
				 clampc(lhs.a - rhs.a));
}

Color operator *(const Color &lhs, const Color &rhs) {
	return Color(clampc(lhs.r * rhs.r),
				 clampc(lhs.g * rhs.g),
				 clampc(lhs.b * rhs.b),
				 clampc(lhs.a * rhs.a));
}

Color &operator +=(Color &lhs, const Color &rhs) {
	return lhs = lhs + rhs;
}

Color &operator -=(Color &lhs, const Color &rhs) {
	return lhs = lhs - rhs;
}

Color &operator *=(Color &lhs, const Color &rhs) {
	return lhs = lhs * rhs;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawInit() {
	_shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
	_color = Color::White;

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	_shader->use();
	drawSetProjection(0.0f, (float)_vgde->windowWidth(), (float)_vgde->windowHeight(), 0.0f, -1.0f, 1.0);
}

glm::mat4 drawGetProjection() {
    return _projection;
}

void drawSetProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
	_projection = glm::ortho<float>(left, right, bottom, top, zNear, zFar);
	_shader->setMat4("projection", _projection);
}

Color drawGetColor() {
	/*
	GLfloat glc[4];
	glGetFloatv(GL_CURRENT_COLOR, glc);

	Color c(glc[0] * 255, glc[1] * 255, glc[2] * 255, glc[3] * 255);
	*/

	return _color;
}

void drawSetColor(const Color &c) {
	_color = c;

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
	float r = _color.glR;
	float g = _color.glG;
	float b = _color.glB;
	float a = _color.glA;

	float verts[] = {
		x, y, r, g, b, a,
		x1, y1, r, g, b, a
	};

    _shader->use();
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, null);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 2));

	glDrawArrays(GL_LINES, 0, 2);
    _shader->stop();
}

void drawLine(const vec2f &pos, const vec2f &pos1) {
	drawLine(pos.x, pos.y, pos1.x, pos1.y);
}

void drawRectangle(float x, float y, float w, float h, bool outline) {
	float r = _color.glR;
	float g = _color.glG;
	float b = _color.glB;
	float a = _color.glA;

	float verts[] = {
            x,     y + h, r, g, b, a,
            x + w, y + h, r, g, b, a,
            x + w, y,     r, g, b, a,
            x,     y,     r, g, b, a
	};

	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	_shader->use();
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, null);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 2));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	if (outline) {
		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, null);
	} else {
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
	}

    _shader->stop();
}

void drawRectangle(const vec2f &pos, const vec2f &size, bool outline) {
	drawRectangle(pos.x, pos.y, size.x, size.y, outline);
}

void drawRectangle(const rectf &rect, bool outline) {
	drawRectangle(rect.x, rect.y, rect.width, rect.height, outline);
}
