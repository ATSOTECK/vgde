#include "draw.h"

#include "shader.h"
#include "vgde.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace {
VGDE *_vgde = VGDE::instance();

Shader *_shader;

glm::mat4 _projection;
GLuint _vbo;
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
	//
}

void drawInit() {
	_shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
	_color = Color::White;

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	_shader->use();
	drawSetProjection(0.0f, (float)_vgde->windowWidth(), (float)_vgde->windowHeight(), 0.0f, -1.0f, 1.0);
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
	float r = _color.r / 255.f;
	float g = _color.g / 255.f;
	float b = _color.b / 255.f;
	float a = _color.a / 255.f;

	float verts[] = {x, y, r, g, b, a,
					 x1, y1, r, g, b, a};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 2));

	glDrawArrays(GL_LINES, 0, 2);
}

void drawLine(const vec2f &pos, const vec2f &pos1) {
	drawLine(pos.x, pos.y, pos1.x, pos1.y);
}
