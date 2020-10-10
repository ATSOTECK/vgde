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

#ifndef __VGDE_COLOR_H
#define __VGDE_COLOR_H

#include "config.h"
#include "vec.h"

class Color {
public:
    explicit Color(uint8 red = 0, uint8 green = 0, uint8 blue = 0, uint8 alpha = 255);

    vec3f vec3gl() const;

    static const Color Black;
    static const Color Black100;
    static const Color Black150;
    static const Color Black200;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color LightBlue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Turquoise;
    static const Color Transparent;
    static const Color None;

    Color &operator=(const Color &other);

    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;

    float glR;
    float glG;
    float glB;
    float glA;
};

bool operator ==(const Color &lhs, const Color &rhs);
bool operator !=(const Color &lhs, const Color &rhs);
Color operator +(const Color &lhs, const Color &rhs);
Color operator -(const Color &lhs, const Color &rhs);
Color operator *(const Color &lhs, const Color &rhs);
Color &operator +=(Color &lhs, const Color &rhs);
Color &operator -=(Color &lhs, const Color &rhs);
Color &operator *=(Color &lhs, const Color &rhs);


#endif //__VGDE_COLOR_H__
