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

#ifndef __VGDE_TRANSFORM_H__
#define __VGDE_TRANSFORM_H__

#include <glm.hpp>

#include "vec.h"

class Transform {
public:
    Transform();

    vec2f position() const;
    void setPosition(const vec2f &pos);
    
    float x() const;
    float y() const;
    
    void setX(float x);
    void setY(float y);

    void move(const vec2f &amount);

    vec2f origin() const;
    void setOrigin(const vec2f &origin);

    float width() const;
    float height() const;
    vec2f size() const;
    void setSize(const vec2f &size);

    vec2f originalSize() const;

    vec2f scale() const;
    void setScale(float scale);
    void setScale(const vec2f &scale);

    void setRotation(float angle);
    void rotate(float speed);
    float rotation() const;

    vec2f center() const;
    
protected:
    vec2f _position;
    vec2f _origin;
    vec2f _porigin;
    vec2f _size;
    vec2f _osize;
    vec2f _scale;
    float _rotation;
    bool _updateVerts;
    bool _updateTransform;
    bool _osizeSet;
    glm::mat4 _transform;
};


#endif //__VGDE_TRANSFORM_H__
