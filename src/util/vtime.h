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

#ifndef __VGDE_TIME_H__
#define __VGDE_TIME_H__

#include "config.h"

class Time {
public:
    Time();
    
    static Time seconds(float seconds);
    static Time milliseconds(int32 milliseconds);
    static Time microseconds(int64 microseconds);

    keep float asSeconds() const;
    keep int32 asMilliseconds() const;
    keep int64 asMicroseconds() const;

    static const Time Zero;
private:
    explicit Time(int64 microseconds);

    int64 _microseconds;
};

bool operator==(const Time &left, const Time &right);
bool operator!=(const Time &left, const Time &right);
bool operator >(const Time &left, const Time &right);
bool operator <(const Time &left, const Time &right);
bool operator>=(const Time &left, const Time &right);
bool operator<=(const Time &left, const Time &right);
Time operator -(const Time &right);
Time operator +(const Time &left, const Time &right);
Time &operator+=(Time &left, const Time &right);
Time operator -(const Time &left, const Time &right);
Time &operator-=(Time &left, const Time &right);
Time operator *(const Time &left, float right);
Time operator *(const Time &left, int64 right);
Time operator *(float left, const Time &right);
Time operator *(int64 left, const Time &right);
Time &operator*=(const Time &left, float right);
Time &operator*=(const Time &left, int64 right);
Time operator /(const Time &left, float right);
Time operator /(const Time &left, int64 right);
Time &operator/=(Time &left, float right);
Time &operator/=(Time &left, int64 right);
Time operator %(const Time &left, const Time &right);
Time &operator%=(Time &left, const Time &right);

#endif //__VGDE_TIME_H__
