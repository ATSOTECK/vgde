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

#ifndef __VGDE_ACTOR_H__
#define __VGDE_ACTOR_H__

#include "graphics/color.h"
#include "graphics/transform.h"

class Actor : public Transform {
public:
    Actor();
    virtual void act(float delta) = 0;
    virtual void draw() = 0;
    
    bool visible() const;
    void setVisible(bool visible);
    
    Color color() const;
    void setColor(const Color &color);
    
    float z() const;
    void setZIndex(float z);
    
    void toBack();
    void toFront();
    
    void drawDebug();
    
protected:
    bool _visible;
    Color _color;
    float _z;
};


#endif //__VGDE_ACTOR_H__
