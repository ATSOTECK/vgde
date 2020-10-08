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
