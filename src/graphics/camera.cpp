#include "camera.h"

#include "gl.h"
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() :
    Transform()
{
    //
}

void Camera::update() {
    _transform = glm::translate(_transform, {_position.x, _position.y, 0.f});
    glMultMatrixf(glm::value_ptr(_transform));
}
