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

#ifndef __VGDE_CLOCK_H__
#define __VGDE_CLOCK_H__

#include "vtime.h"

class Clock {
public:
    Clock();

    static Time time();
    static float timeAsSeconds();
    static int32 timeAsMilliseconds();
    static int64 timeAsMicroseconds();

    static int hour(bool use12hr = false);
    static int minute();
    static int second();
    static int millisecond();

    static bool isAM();
    static bool isPM();

    Time elapsed() const;
    float elapsedAsSeconds() const;
    int32 elapsedAsMilliseconds() const;
    int64 elapsedAsMicroseconds() const;

    Time restart();
    Time startTime() const;

private:
    Time _startTime;
};


#endif //__VGDE_CLOCK_H__
