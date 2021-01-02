/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020-2021 Skyler Burwell
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

#include "draw.h"

#include "shader.h"
#include "vgde.h"
#include "util/resourceManager.h"
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
	drawSetClearColor(Color::Black);
}

glm::mat4 drawGetProjection() {
    return _projection;
}

void drawSetProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
	drawSetProjection(glm::ortho<float>(left, right, bottom, top, zNear, zFar));
}

void drawSetProjection(const glm::mat4 &proj) {
    _projection = proj;
    _shader->setMat4("projection", _projection, true);
    _textShader->setMat4("projection", _projection, true);

    ResourceManager::instance()->updateShaderProjections();

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

void drawSetAlpha(uint8 a) {
	Color c = drawGetColor();
	c.a = a;
	c.glA = (float)a / 255;

	drawSetColor(c);
}

int drawGetFontSize() {
    return _defaultFont->size();
}

void drawSetFontSize(int size) {
    _defaultFont->setSize(size);
}

void drawVerts(float *verts, int count, bool outline) {
    _shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, (VERT_SIZE * count) * sizeof(float), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * VERT_SIZE, null);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * VERT_SIZE, (void*)(sizeof(float) * 2));

    if (outline) {
        glDrawArrays(GL_LINE_STRIP, 0, count);
    } else {
        glDrawArrays(GL_POLYGON, 0, count);
    }
    _shader->stop();
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

    drawVerts(verts,2, true);
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

void drawCircle(float x, float y, float r, int sides, bool outline) {
    if (sides < 0) {
        sides = 1;
    }
    float angleShift = ((float)V_TAU / (float)sides);
    float phi = 0.f;

    float *verts = new float[VERT_SIZE * (sides + 1)];
    for (int i = 0; i < sides; ++i, phi += angleShift) {
        verts[VERT_SIZE * i]     = x + r * cos(phi);
        verts[VERT_SIZE * i + 1] = y + r * sin(phi);
        verts[VERT_SIZE * i + 2] = _color.glR;
        verts[VERT_SIZE * i + 3] = _color.glG;
        verts[VERT_SIZE * i + 4] = _color.glB;
        verts[VERT_SIZE * i + 5] = _color.glA;
    }

    verts[VERT_SIZE * sides]     = verts[0];
    verts[VERT_SIZE * sides + 1] = verts[1];
    verts[VERT_SIZE * sides + 2] = _color.glR;
    verts[VERT_SIZE * sides + 3] = _color.glG;
    verts[VERT_SIZE * sides + 4] = _color.glB;
    verts[VERT_SIZE * sides + 5] = _color.glA;

    drawVerts(verts, sides + 1, outline);

    delete [] verts;
}

void drawArc(float x, float y, float r, float a, float a1, int sides, bool radians) {
    if (sides == 0 || a == a1) {
        return;
    }

    if (!radians) {
        a = degToRad(a);
        a1 = degToRad(a1);
    }

    if (fabs(a - a1) >= V_TAU) {
        drawCircle(x, y, r, sides, true);
        return;
    }

    float angleShift = (a - a1) / (float)sides;
    if (angleShift == 0) {
        return;
    }

    float phi = a;
    float *verts = new float[VERT_SIZE * (sides + 1)];

    for (int i = 0; i <= sides; ++i, phi += angleShift) {
        verts[VERT_SIZE * i]     = x + r * cos(phi);
        verts[VERT_SIZE * i + 1] = y + r * sin(phi);
        verts[VERT_SIZE * i + 2] = _color.glA;
        verts[VERT_SIZE * i + 3] = _color.glG;
        verts[VERT_SIZE * i + 4] = _color.glB;
        verts[VERT_SIZE * i + 5] = _color.glA;
    }

    drawVerts(verts, sides + 1, true);

}

void drawSlice(float x, float y, float r, float a, float a1, int sides, bool outline, bool radians) {
    if (sides == 0 || a == a1) {
        return;
    }

    if (!radians) {
        a = degToRad(a);
        a1 = degToRad(a1);
    }

    if (fabs(a - a1) >= V_TAU) {
        drawCircle(x, y, r, sides, outline);
        return;
    }

    float angleShift = (a - a1) / (float)sides;
    if (angleShift == 0) {
        return;
    }

    float phi = a;
    int vertsSize = (sides + 3) * VERT_SIZE;
    float *verts = new float[vertsSize];
    verts[0] = verts[vertsSize - VERT_SIZE] = x;
    verts[1] = verts[vertsSize - 5] = y;
    verts[2] = verts[vertsSize - 4] = _color.glR;
    verts[3] = verts[vertsSize - 3] = _color.glG;
    verts[4] = verts[vertsSize - 2] = _color.glB;
    verts[5] = verts[vertsSize - 1] = _color.glA;

    for (int i = 0; i <= sides; ++i, phi += angleShift) {
        verts[VERT_SIZE * (i + 1)] = x + r * cos(phi);
        verts[VERT_SIZE * (i + 1) + 1] = y + r * sin(phi);
        verts[VERT_SIZE * (i + 1) + 2] = _color.glA;
        verts[VERT_SIZE * (i + 1) + 3] = _color.glG;
        verts[VERT_SIZE * (i + 1) + 4] = _color.glB;
        verts[VERT_SIZE * (i + 1) + 5] = _color.glA;
    }

    drawVerts(verts, sides + 3, outline);

}

float drawText(const String &txt, float x, float y, float scale, const Color &color, Font *font) {
    if (font == null) {
        return 0;
    }
    
    _textShader->use();
    _textShader->setVec3f("textColor", color.vec3gl());

    float w = font->draw(txt, x, y, scale, _textShader, color);

    _textShader->stop();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return w;
}

float drawText(const String &txt, float x, float y, float scale, const Color &color) {
    return drawText(txt, x, y, scale, color, _defaultFont);
}
