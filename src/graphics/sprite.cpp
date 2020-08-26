#include "sprite.h"

#include "config.h"
#include "draw.h"

#include <iostream>

namespace {
const std::string defaultVertexShader =
"#version 330 core\n"
"layout (location = 0) in vec2 vert;\n"
"layout (location = 1) in vec2 vertTexCoord;\n"
"uniform mat4 projection;"
"\n"
"out vec2 fragTexCoord;\n"
"\n"
"void main() {\n"
"   fragTexCoord = vertTexCoord;\n"
"	gl_Position = projection * vec4(vert, 0, 1.0);\n"
"}";

const std::string defaultFragmentShader =
"#version 330 core\n"
"uniform sampler2D tex;\n"
"in vec2 fragTexCoord;\n"
"out vec4 finalColor;\n"
"\n"
"void main() {\n"
"	finalColor = texture(tex, fragTexCoord);\n"
"}";

const GLuint elements[] ={
    0, 1, 2,
    2, 3, 0
};
}

Sprite::Sprite(const std::string &spr) {
    _texture = new Texture(spr);

    if (_texture->width() == 0) {
        return;
    }

    init();
}

Sprite::Sprite(Texture *texture) {
    _texture = texture;

    init();
}

void Sprite::init() {
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    _width = _texture->width();
    _height = _texture->height();

    _position = vec2i();
    _scale = 1.f;

    _shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
    setVerts();

    _shader->use();
    glm::mat4 projection = drawGetProjection();
    _shader->setMat4("projection", projection);
}

void Sprite::setVerts() {
    float x = _position.x;
    float y = _position.y;
    float w = _width;
    float h = _height;

    _verts = {
            //  Position   Texcoords
            x,      y + h, 0.0f, 0.0f, // Top-left
            x + w,  y + h, 1.0f, 0.0f, // Top-right
            x + w,  y,     1.0f, 1.0f, // Bottom-right
            x,      y,     0.0f, 1.0f  // Bottom-left
    };
}

void Sprite::draw() {

    /*
    float verts[] = {
            //  Position  Color                   Texcoords
            x,      y + h, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
            x + w,  y + h, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Top-right
            x + w,  y,     0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
            x,      y,     1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };*/

    _shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &_verts[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(sizeof(float) * 2));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), elements, GL_STATIC_DRAW);

    _texture->bind();
    //_shader->setInt("tex", 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);

	_texture->unbind();
	_shader->stop();

    glBindVertexArray(0);
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
	setVerts();
}

vec2i Sprite::size() const {
    return {_width, _height};
}

void Sprite::setSize(const vec2i &size) {
    _width = size.x;
    _height = size.y;
    setVerts();
}

float Sprite::scale() const {
    return _scale;
}

void Sprite::setScale(float scale) {
    _scale = scale;
    _width = _texture->width() * _scale;
    _height = _texture->height() * _scale;
    setVerts();
}
