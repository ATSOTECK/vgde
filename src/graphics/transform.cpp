#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform() :
    _position(0.f),
    _origin(0.f),
    _porigin(0.f),
    _size(0.f),
    _osize(0.f),
    _scale(1.f),
    _rotation(0.f),
    _updateVerts(false),
    _updateTransform(false),
    _osizeSet(false),
    _transform(glm::mat4(1.f))
{
    //
}

float Transform::width() const {
    return _size.x;
}

float Transform::height() const {
    return _size.y;
}

vec2f Transform::position() const {
    return _position;
}

void Transform::setPosition(const vec2f &pos) {
    _position = pos;
    _updateVerts = true;
}

float Transform::x() const {
    return _position.x;
}

float Transform::y() const {
    return _position.y;
}

void Transform::setX(float x) {
    _position.x = x;
}

void Transform::setY(float y) {
    _position.y = y;
}

void Transform::move(const vec2f &amount) {
    _position += amount;
    _updateVerts = true;
}

vec2f Transform::origin() const {
    return _origin;
}

void Transform::setOrigin(const vec2f &origin) {
    _origin = origin;
    _porigin = origin;
    _updateVerts = true;
}

vec2f Transform::size() const {
    return _size;
}

void Transform::setSize(const vec2f &size) {
    _size = size;
    _updateVerts = true;

    if (!_osizeSet) {
        _osize = _size;
        _osizeSet = true;
    }
}

vec2f Transform::originalSize() const {
    return _osize;
}

vec2f Transform::scale() const {
    return _scale;
}

void Transform::setScale(float scale) {
    setScale({scale, scale});
}

void Transform::setScale(const vec2f &scale) {
    _scale = scale;
    _size.x = _osize.x * _scale.x;
    _size.y = _osize.y * _scale.y;

    _origin = _porigin * scale;

    _updateVerts = true;
}

void Transform::setRotation(float angle) {
    _rotation = angle;
    _transform = glm::mat4(1.f);
    _transform = glm::translate(_transform, {_position.x, _position.y, 0.f});
    _transform = glm::rotate(_transform, glm::radians(_rotation), {0, 0, 1});
    _transform = glm::translate(_transform, {-_position.x, -_position.y, 0.f});
    _updateTransform = true;
}

void Transform::rotate(float speed) {
    setRotation(_rotation + speed);
}

float Transform::rotation() const {
    return _rotation;
}

vec2f Transform::center() const {
    return _size / 2.f;
}
