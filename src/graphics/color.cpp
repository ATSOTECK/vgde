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
    glR = r / 255.f;
    glG = g / 255.f;
    glB = b / 255.f;
    glA = a / 255.f;
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
