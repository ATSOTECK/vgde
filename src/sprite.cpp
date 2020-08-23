#include "sprite.h"

#include "config.h"

#include <iostream>

namespace {
const std::string defaultFragmentShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"\n"
"void main() {\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"	TexCoord = aTexCoord;\n"
"}";

const std::string defaultVertexShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"\n"
"uniform sampler2D ourTexture;\n"
"\n"
"void main() {\n"
"	FragColor = texture(ourTexture, TexCoord);\n"
"}";

const GLfloat verts[] = {
    //  Position  Color                   Texcoords
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
};

const GLuint elements[] ={
    0, 1, 2,
    2, 3, 0
};
}

Sprite::Sprite(const std::string &spr) {
    _texture = new Texture(spr);
    _texture->bind();

    if (_texture->width() == 0) {
        return;
    }

    _width = _texture->width();
    _height = _texture->height();

    _position = vec2i();

    _shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
    init();
}

Sprite::Sprite(Texture *texture) {
    _texture = texture;
    _texture->bind();

    _width = _texture->width();
    _height = _texture->height();

    _position = vec2i();

    _shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
}

void Sprite::init() {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Sprite::draw() {
    _shader->use();
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), elements, GL_STATIC_DRAW);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
}

int Sprite::width() const {
	return _width;
}

int Sprite::height() const {
	return _height;
}

vec2i Sprite::position() const {
	return _position;
}

void Sprite::setPosition(const vec2i &pos) {
	_position = pos;
}
