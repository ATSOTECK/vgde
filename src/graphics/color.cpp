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
const Color Color::None(0, 0, 0, 0, false);

Color::Color(uint8 red, uint8 green, uint8 blue, uint8 alpha, bool valid) :
    _r(red),
    _g(green),
    _b(blue),
    _a(alpha),
    _valid(valid)
{
    _glR = (float)_r / 255.f;
    _glG = (float)_g / 255.f;
    _glB = (float)_b / 255.f;
    _glA = (float)_a / 255.f;
}

vec3f Color::vec3gl() const {
    return {_glR, _glG, _glB};
}

SimpleColor Color::simple() const {
    return {(float)_r / 255.f, (float)_g / 255.f, (float)_b / 255.f, (float)_a / 255.f};
}

void Color::set(uint8 red, uint8 green, uint8 blue, uint8 alpha) {
    _r = red;
    _g = green;
    _b = blue;
    _a = alpha;
    
    _glR = (float)_r / 255.f;
    _glG = (float)_g / 255.f;
    _glB = (float)_b / 255.f;
    _glA = (float)_a / 255.f;
}

Color Color::random() {
    return Color(randomi(255), randomi(255), randomi(255));
}

Color &Color::operator= (const Color &other) {
    if (this == &other) {
        return *this;
    }

    _r = other._r;
    _g = other._g;
    _b = other._b;
    _a = other._a;

    _glR = other._glR;
    _glG = other._glG;
    _glB = other._glB;
    _glA = other._glA;
    
    _valid = other._valid;

    return *this;
}

void Color::r(uint8 r) {
    _r = r;
    _glR = (float)_r / 255.f;
}

uint8 Color::r() const {
    return _r;
}

void Color::g(uint8 g) {
    _g = g;
    _glG = (float)_g / 255.f;
}

uint8 Color::g() const {
    return _g;
}

void Color::b(uint8 b) {
    _b = b;
    _glB = (float)_b / 255.f;
}

uint8 Color::b() const {
    return _b;
}

void Color::a(uint8 a) {
    _a = a;
    _glA = (float)_a / 255.f;
}

uint8 Color::a() const {
    return _a;
}

float Color::glR() const {
    return _glR;
}

float Color::glG() const {
    return _glG;
}

float Color::glB() const {
    return _glB;
}

float Color::glA() const {
    return _glA;
}

bool operator ==(const Color &lhs, const Color &rhs) {
    return (lhs._r == rhs._r) &&
           (lhs._g == rhs._g) &&
           (lhs._b == rhs._b) &&
           (lhs._a == rhs._a) &&
           (lhs._valid == rhs._valid);
}

bool operator !=(const Color &lhs, const Color &rhs) {
    return !(lhs == rhs);
}

Color operator +(const Color &lhs, const Color &rhs) {
    return Color(clampc(lhs._r + rhs._r),
                 clampc(lhs._g + rhs._g),
                 clampc(lhs._b + rhs._b),
                 clampc(lhs._a + rhs._a));
}

Color operator -(const Color &lhs, const Color &rhs) {
    return Color(clampc(lhs._r - rhs._r),
                 clampc(lhs._g - rhs._g),
                 clampc(lhs._b - rhs._b),
                 clampc(lhs._a - rhs._a));
}

Color operator *(const Color &lhs, const Color &rhs) {
    return Color(clampc(lhs._r * rhs._r),
                 clampc(lhs._g * rhs._g),
                 clampc(lhs._b * rhs._b),
                 clampc(lhs._a * rhs._a));
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

std::ostream &operator <<(std::ostream &os, Color &color) {
    os << "(" << (int)color._r << ", " << (int)color._g << ", " << (int)color._b << ", " << (int)color._a << ") " << color._valid;
    
    return os;
}

std::ostream &operator <<(std::ostream &os, const Color &color) {
    os << "(" << (int)color._r << ", " << (int)color._g << ", " << (int)color._b << ", " << (int)color._a << ") " << color._valid;
    
    return os;
}
