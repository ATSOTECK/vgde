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

    return microseconds(1000000 * time.QuadPart / freq.QuadPart);
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
