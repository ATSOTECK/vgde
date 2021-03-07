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

#ifndef __VGDE_COLOR_H
#define __VGDE_COLOR_H

#include "config.h"
#include "vec.h"

#include <iostream>

struct SimpleColor {
    float r;
    float g;
    float b;
    float a;
};

class Color {
public:
    explicit Color(uint8 red = 0, uint8 green = 0, uint8 blue = 0, uint8 alpha = 255, bool valid = true);

    keep vec3f vec3gl() const;
    keep SimpleColor simple() const;
    
    void set(uint8 red = 0, uint8 green = 0, uint8 blue = 0, uint8 alpha = 255);

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
    
    static Color random();

    Color &operator=(const Color &other);
    
    void r(uint8 r);
    keep uint8 r() const;
    
    void g(uint8 g);
    keep uint8 g() const;
    
    void b(uint8 b);
    keep uint8 b() const;
    
    void a(uint8 a);
    keep uint8 a() const;
    
    keep float glR() const;
    keep float glG() const;
    keep float glB() const;
    keep float glA() const;
    
private:
    uint8 _r;
    uint8 _g;
    uint8 _b;
    uint8 _a;
    
    float _glR;
    float _glG;
    float _glB;
    float _glA;
    
    friend bool operator ==(const Color &lhs, const Color &rhs);
    friend Color operator +(const Color &lhs, const Color &rhs);
    friend Color operator -(const Color &lhs, const Color &rhs);
    friend Color operator *(const Color &lhs, const Color &rhs);
    
    friend std::ostream &operator <<(std::ostream &os, Color &color);
    friend std::ostream &operator <<(std::ostream &os, const Color &color);
    
    bool _valid;
};

bool operator ==(const Color &lhs, const Color &rhs);
bool operator !=(const Color &lhs, const Color &rhs);
Color operator +(const Color &lhs, const Color &rhs);
Color operator -(const Color &lhs, const Color &rhs);
Color operator *(const Color &lhs, const Color &rhs);
Color &operator +=(Color &lhs, const Color &rhs);
Color &operator -=(Color &lhs, const Color &rhs);
Color &operator *=(Color &lhs, const Color &rhs);

std::ostream &operator <<(std::ostream &os, Color &color);
std::ostream &operator <<(std::ostream &os, const Color &color);


#endif //__VGDE_COLOR_H__
