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

#include "color.h"
#include "util/vmath.h"

const Color Color::Black(0, 0, 0);
const Color Color::Black100(0, 0, 0, 100);
const Color Color::Black150(0, 0, 0, 150);
const Color Color::Black200(0, 0, 0, 200);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::LightBlue(72, 118, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Turquoise(64, 244, 208);
const Color Color::Transparent(0, 0, 0, 0);
const Color Color::None(0, 0, 0, 0);

Color::Color(uint8 red, uint8 green, uint8 blue, uint8 alpha) :
        r(red),
        g(green),
        b(blue),
        a(alpha)
{
    glR = (float)r / 255.f;
    glG = (float)g / 255.f;
    glB = (float)b / 255.f;
    glA = (float)a / 255.f;
}

vec3f Color::vec3gl() const {
    return {glR, glG, glB};
}

Color &Color::operator= (const Color &other) {
    if (this == &other) {
        return *this;
    }

    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;

    glR = other.glR;
    glG = other.glG;
    glB = other.glB;
    glA = other.glA;

    return *this;
}

bool operator ==(const Color &lhs, const Color &rhs) {
    return (lhs.r == rhs.r) &&
           (lhs.g == rhs.g) &&
           (lhs.b == rhs.b) &&
           (lhs.a == rhs.a);
}

bool operator !=(const Color &lhs, const Color &rhs) {
    return !(lhs == rhs);
}

Color operator +(const Color &lhs, const Color &rhs) {
    return Color(clampc(lhs.r + rhs.r),
                 clampc(lhs.g + rhs.g),
                 clampc(lhs.b + rhs.b),
                 clampc(lhs.a + rhs.a));
}

Color operator -(const Color &lhs, const Color &rhs) {
    return Color(clampc(lhs.r - rhs.r),
                 clampc(lhs.g - rhs.g),
                 clampc(lhs.b - rhs.b),
                 clampc(lhs.a - rhs.a));
}

Color operator *(const Color &lhs, const Color &rhs) {
    return Color(clampc(lhs.r * rhs.r),
                 clampc(lhs.g * rhs.g),
                 clampc(lhs.b * rhs.b),
                 clampc(lhs.a * rhs.a));
}

Color &operator +=(Color &lhs, const Color &rhs) {
    return lhs = lhs + rhs;
}

Color &operator -=(Color &lhs, const Color &rhs) {
    return lhs = lhs - rhs;
}

Color &operator *=(Color &lhs, const Color &rhs) {
    return lhs = lhs * rhs;
}
