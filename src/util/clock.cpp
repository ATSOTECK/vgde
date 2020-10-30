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

#include "clock.h"

#include <Windows.h>

Clock::Clock() :
    _startTime(time())
{
    //
}

Time Clock::time() {
    HANDLE currentThread = GetCurrentThread();
    DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

    static LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    SetThreadAffinityMask(currentThread, previousMask);

    return Time::microseconds(1000000 * time.QuadPart / freq.QuadPart);
}

float Clock::timeAsSeconds() {
    return time().asSeconds();
}

int32 Clock::timeAsMilliseconds() {
    return time().asMilliseconds();
}

int64 Clock::timeAsMicroseconds() {
    return time().asMicroseconds();
}

int Clock::hour(bool use12hr) {
    SYSTEMTIME time;
    GetLocalTime(&time);

    if (time.wHour > 12 && use12hr) {
        time.wHour -= 12;
    } else if (time.wHour == 0 && use12hr) {
        time.wHour = 12;
    }

    return time.wHour;
}

int Clock::minute() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    return time.wMinute;
}

int Clock::second() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    return time.wSecond;
}

int Clock::millisecond() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    return time.wMilliseconds;
}

bool Clock::isAM() {
    int h = hour(false);
    int m = minute();
    int s = second();

    return !(h >= 12 && (m > 0 || s > 0));
}

bool Clock::isPM() {
    return !isAM();
}

Time Clock::elapsed() const {
    return time() - _startTime;
}

float Clock::elapsedAsSeconds() const {
    return elapsed().asSeconds();
}

int32 Clock::elapsedAsMilliseconds() const {
    return elapsed().asMilliseconds();
}

int64 Clock::elapsedAsMicroseconds() const {
    return elapsed().asMicroseconds();
}

Time Clock::restart() {
    Time now = time();
    Time elapsed = now - _startTime;
    _startTime = now;

    return elapsed;
}

Time Clock::startTime() const {
    return _startTime;
}
