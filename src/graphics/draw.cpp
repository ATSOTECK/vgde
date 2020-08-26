#include "draw.h"

#include "font.h"
#include "shader.h"
#include "vgde.h"
#include "util/vmath.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace {
VGDE *_vgde = VGDE::instance();

Shader *_shader;
Shader *_textShader;

glm::mat4 _projection;
GLuint _vbo;
GLuint _ibo;
Color _color;

Font *_defaultFont;

const std::string defaultVertexShader =
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

const std::string defaultFragmentShader =
"#version 330 core\n"
"out vec4 outColor;\n"
"in vec4 color;"
"void main()\n"
"{\n"
"   outColor = color;\n"
"}";

const std::string textVertShader =
"#version 330 core\n"
"layout (location = 0) in vec2 vertex;\n"
"layout (location = 1) in vec2 vertTexCoord;\n"
"out vec2 texCoords;\n"
"uniform mat4 projection;\n"
"\n"
"void main() {\n"
"   gl_Position = projection * vec4(vertex.xy, 0, 1.0);\n"
"   texCoords = vertTexCoord;\n"
"}";

const std::string textFragShader =
"#version 330 core\n"
"in vec2 texCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"\n"
"void main() {\n"
"   vec4 sampled = vec4(1, 1, 1, texture(text, texCoords).r);\n"
"   color = vec4(textColor, 1) * sampled;\n"
"}";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawInit() {
	_shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
	_textShader = new Shader(textVertShader, textFragShader, false);
	_color = Color::White;

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	_defaultFont = new Font("liberation-mono.ttf");

	drawSetProjection(0.0f, (float)_vgde->windowWidth(), (float)_vgde->windowHeight(), 0.0f, -1.0f, 1.0);
}

glm::mat4 drawGetProjection() {
    return _projection;
}

void drawSetProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
	_projection = glm::ortho<float>(left, right, bottom, top, zNear, zFar);
	_shader->use();
	_shader->setMat4("projection", _projection);
	_textShader->use();
	_textShader->setMat4("projection", _projection);

	glUseProgram(0);
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

	glColor4f(c.glR, c.glG, c.glB, c.glA);
}

Color drawGetClearColor() {
	GLfloat glc[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, glc);

	Color c(glc[0] * 255, glc[1] * 255, glc[2] * 255, glc[3] * 255);

	return c;
}

void drawSetClearColor(const Color &c) {
	glClearColor(c.glR, c.glG, c.glB, c.glA);
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
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
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

	uint indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	_shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, null);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 2));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
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

void drawText(const std::string &txt, float x, float y, float scale, const Color &color) {
    _textShader->use();
    _textShader->setVec3f("textColor", color.vec3gl());

    _defaultFont->draw(txt, x, y, scale);

    _textShader->stop();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
