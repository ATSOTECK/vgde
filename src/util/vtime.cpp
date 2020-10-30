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

#include "vtime.h"

const Time Time::Zero;

Time::Time() :
    _microseconds(0)
{
    //
}

float Time::asSeconds() const {
    return _microseconds / 1000000.f;
}

int32 Time::asMilliseconds() const {
    return _microseconds / 1000;
}

int64 Time::asMicroseconds() const {
    return _microseconds;
}

Time::Time(int64 microseconds) :
    _microseconds(microseconds)
{
    //
}

Time Time::seconds(float seconds) {
    return Time(seconds * 1000000);
}

Time Time::milliseconds(int32 milliseconds) {
    return Time(milliseconds * 1000);
}

Time Time::microseconds(int64 microseconds) {
    return Time(microseconds);
}

bool operator==(const Time &left, const Time &right) {
    return left.asMicroseconds() == right.asMicroseconds();
}

bool operator!=(const Time &left, const Time &right) {
    return left.asMicroseconds() != right.asMicroseconds();
}

bool operator >(const Time &left, const Time &right) {
    return left.asMicroseconds() > right.asMicroseconds();
}

bool operator <(const Time &left, const Time &right) {
    return left.asMicroseconds() < right.asMicroseconds();
}

bool operator>=(const Time &left, const Time &right) {
    return left.asMicroseconds() >= right.asMicroseconds();
}

bool operator<=(const Time &left, const Time &right) {
    return left.asMicroseconds() <= right.asMicroseconds();
}

Time operator -(const Time &right) {
    return Time::microseconds(-right.asMicroseconds());
}
Time operator +(const Time &left, const Time &right) {
    return Time::microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time &operator+=(Time &left, const Time &right) {
    return left = left + right;
}

Time operator -(const Time &left, const Time &right) {
    return Time::microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time &operator-=(Time &left, const Time &right) {
    return left = left - right;
}

Time operator *(const Time &left, float right) {
    return Time::seconds(left.asSeconds() * right);
}

Time operator *(const Time &left, int64 right) {
    return Time::microseconds(left.asMicroseconds() * right);
}

Time operator *(float left, const Time &right) {
    return right * left;
}

Time operator *(int64 left, const Time &right) {
    return right * left;
}

Time &operator*=(Time &left, float right) {
    return left = left * right;
}

Time &operator*=(Time &left, int64 right) {
    return left = left * right;
}

Time operator /(const Time &left, float right) {
    return Time::seconds(left.asSeconds() / right);
}

Time operator /(const Time &left, int64 right) {
    return Time::microseconds(left.asMicroseconds() / right);
}

Time &operator/=(Time &left, float right) {
    return left = left / right;
}

Time &operator/=(Time &left, int64 right) {
    return left = left / right;
}

Time operator %(const Time &left, const Time &right) {
    return Time::microseconds(left.asMicroseconds() % right.asMicroseconds());
}
Time &operator%=(Time &left, const Time &right) {
    return left = left % right;
}
