#include "sprite.h"

#include "config.h"
#include "draw.h"
#include "util/resourceManager.h"

#include "vgde.h"

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
"	gl_Position = transform * projection * vec4(vert, 0, 1.0);\n"
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

Sprite::Sprite(const std::string &spr) {
    _texture = ResourceManager::instance()->loadTexture(spr);

    if (_texture == null || _texture->width() == 0 || _texture->height() == 0) {
        return;
    }

    init();
}

Sprite::Sprite(Texture *texture) {
    _texture = texture;

    init();
}

Sprite::~Sprite() {
    ResourceManager::instance()->unloadTexture(_texture);
    _shader->stop();
    ResourceManager::instance()->removeShader(_shader);
    delete _shader;
}

void Sprite::init() {
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    _size = _texture->size();

    _position = 0.f;
    _origin = 0.f;
    _scale = 1.f;

    _shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
    setVerts();

    _transform = glm::mat4(1.f);
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

    /*
    float verts[] = {
            //  Position  Color                   Texcoords
            x,      y + h, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
            x + w,  y + h, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Top-right
            x + w,  y,     0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
            x,      y,     1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };*/

    _shader->use();
    //VGDE *v = VGDE::instance();
    //glViewport(0, 0, v->windowWidth(), v->windowHeight());
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

float Sprite::width() const {
	return _size.x;
}

float Sprite::height() const {
	return _size.y;
}

vec2f Sprite::position() const {
	return _position;
}

void Sprite::setPosition(const vec2f &pos) {
	_position = pos;
	setVerts();
}

vec2f Sprite::size() const {
    return _size;
}

void Sprite::setSize(const vec2f &size) {
    _size = size;
    setVerts();
}

vec2f Sprite::scale() const {
    return _scale;
}

void Sprite::setScale(const vec2f &scale) {
    _scale = scale;
    _size.x = _texture->width() * _scale.x;
    _size.y = _texture->height() * _scale.y;
    setVerts();
}

void Sprite::setRotation(float angle) {
    //TODO(Skyler): Figure out this mess.
    //This converts position to clipping space from screen sapce.
    glm::mat4 proj = drawGetProjection();
    glm::vec4 vp({_position.x, _position.y, 0.f, 1.f});
    glm::vec4 result = proj * vp;

    _transform = glm::mat4(1.f);
    _transform = glm::translate(_transform, {result.x, result.y, 0.f});
    _transform = glm::rotate(_transform, glm::radians(angle), {0, 0, 1});
    _transform = glm::translate(_transform, {-result.x, -result.y, 0.f});
    _shader->setMat4("transform", _transform, true);
}

void Sprite::setOrigin(const vec2f &origin) {
    _origin = origin;
    setVerts();
}
