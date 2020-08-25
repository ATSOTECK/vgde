#include "draw.h"

#include "shader.h"
#include "vgde.h"
#include "util/vmath.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <map>

namespace {
VGDE *_vgde = VGDE::instance();

Shader *_shader;
Shader *_textShader;
uint _tvao;
uint _tvbo;

glm::mat4 _projection;
GLuint _vbo;
GLuint _ibo;
Color _color;

std::map<char, Character> _chars;

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
	_textShader = new Shader(textVertShader, textFragShader, false);
	_color = Color::White;

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	drawSetProjection(0.0f, (float)_vgde->windowWidth(), (float)_vgde->windowHeight(), 0.0f, -1.0f, 1.0);

	//TODO(Skyler): Move to dedicated font file.
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
	    vgderr("Could not init freetype!");
	}

	FT_Face face;
	if (FT_New_Face(ft, "liberation-mono.ttf", 0, &face)) {
	    vgderr("Failed to load font!");
	}

	FT_Set_Pixel_Sizes(face, 0, 24);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (uchar c = 0; c < 128; ++c) {
	    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
	        vgderr("Failed to load glyph!");
	        return;
	    }

	    uint texture;
	    glGenTextures(1, &texture);
	    glBindTexture(GL_TEXTURE_2D, texture);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
	            0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
                texture,
                {(int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows},
                {face->glyph->bitmap_left, face->glyph->bitmap_top},
                (uint)face->glyph->advance.x
        };
        _chars.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

	glBindTexture(GL_TEXTURE_2D, 0);
    glGenVertexArrays(1, &_tvao);
    glGenBuffers(1, &_tvbo);
    glBindVertexArray(_tvao);
    glBindBuffer(GL_ARRAY_BUFFER, _tvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, null, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
	glUseProgram(0);
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

	unsigned int indicies[] = {
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
    _textShader->setVec3f("textColor", {color.glR, color.glG, color.glB});
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_tvao);

    std::string::const_iterator c;
    for (c = txt.begin(); c != txt.end(); ++c) {
        Character ch = _chars[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.bearing.y * scale) + 24; //TODO(Skyler): '24' is the font size. Move this over to a font file.
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float verts[] = {
                //  Position         Texcoords
                xpos,      ypos + h, 0.0f, 1.0f, // Top-left
                xpos + w,  ypos + h, 1.0f, 1.0f, // Top-right
                xpos + w,  ypos,     1.0f, 0.0f, // Bottom-right
                xpos,      ypos,     0.0f, 0.0f  // Bottom-left
        };

        unsigned int indicies[] = {
                0, 1, 2,
                2, 3, 0
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, _tvbo);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), verts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(sizeof(float) * 2));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
        x+= (ch.advance >> 6) * scale;
    }

    _textShader->stop();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
