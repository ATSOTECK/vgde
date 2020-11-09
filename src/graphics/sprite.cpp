/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020 Skyler Burwell
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

#include "sprite.h"

#include "config.h"
#include "draw.h"
#include "util/resourceManager.h"

namespace {
const std::string defaultVertexShader =
"#version 330 core\n"
"layout (location = 0) in vec2 vert;\n"
"layout (location = 1) in vec2 vertTexCoord;\n"
"uniform mat4 projection;\n"
"uniform mat4 transform;\n"
"\n"
"out vec2 fragTexCoord;\n"
"\n"
"void main() {\n"
"   fragTexCoord = vertTexCoord;\n"
"	gl_Position = projection * transform * vec4(vert, 0, 1.0);\n"
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

//TODO(Skyler): Fix sprites not working with the viewport.

Sprite::Sprite(const std::string &spr) :
    Transform()
{
    _texture = ResourceManager::instance()->loadTexture(spr);

    if (_texture == null || _texture->width() == 0 || _texture->height() == 0) {
        return;
    }

    init();
}

Sprite::Sprite(Texture *texture) :
    Transform()
{
    _texture = texture;

    init();
}

Sprite::~Sprite() {
    ResourceManager::instance()->unloadTexture(_texture);
    _shader->stop();
    ResourceManager::instance()->removeShader(_shader);
}

void Sprite::init() {
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    setSize(_texture->size());

    _shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
    setVerts();

    _shader->setMat4("transform", _transform, true);

    glm::mat4 projection = drawGetProjection();
    _shader->setMat4("projection", projection, true);

    ResourceManager::instance()->addShader(_shader);
}

void Sprite::setVerts() {
    float x = _position.x - _origin.x;
    float y = _position.y - _origin.y;
    float w = _size.x;
    float h = _size.y;

    _verts = {
            //  Position   Texcoords
            x,      y + h, 0.0f, 0.0f, // Top-left
            x + w,  y + h, 1.0f, 0.0f, // Top-right
            x + w,  y,     1.0f, 1.0f, // Bottom-right
            x,      y,     0.0f, 1.0f  // Bottom-left
    };
}

void Sprite::draw() {
    if (_texture == null || _texture->width() == 0 || _texture->height() == 0) {
        return;
    }

    if (_updateVerts) {
        setVerts();
        _updateVerts = false;
    }

    if (_updateTransform) {
        _shader->setMat4("transform", _transform, true);
        _updateTransform = false;
    }

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);

	_texture->unbind();
	_shader->stop();

    glBindVertexArray(0);
}
