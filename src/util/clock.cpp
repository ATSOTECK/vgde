#include "clock.h"

#include <Windows.h>

Clock::Clock() :
    _startTime(getTime())
{
    //
}

Time Clock::getTime() {
    HANDLE currentThread = GetCurrentThread();
    DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

    static LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    SetThreadAffinityMask(currentThread, previousMask);

    return microseconds(1000000 * time.QuadPart / freq.QuadPart);
}

float Clock::getTimeAsSeconds() {
    return getTime().asSeconds();
}

int32 Clock::getTimeAsMilliseconds() {
    return getTime().asMilliseconds();
}

int64 Clock::getTimeAsMicroseconds() {
    return getTime().asMicroseconds();
}

int Clock::getHour(bool use12hr) {
    SYSTEMTIME time;
    GetLocalTime(&time);

    if (time.wHour > 12 && use12hr) {
        time.wHour -= 12;
    } else if (time.wHour == 0 && use12hr) {
        time.wHour = 12;
    }

    return time.wHour;
}

int Clock::getMinute() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    return time.wMinute;
}

int Clock::getSecond() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    return time.wSecond;
}

int Clock::getMillisecond() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    return time.wMilliseconds;
}

bool Clock::isAM() {
    int hour = getHour(false);
    int minute = getMinute();
    int second = getSecond();

    return !(hour >= 12 && (minute > 0 || second > 0));
}

bool Clock::isPM() {
    return !isAM();
}

Time Clock::getElapsed() const {
    return getTime() - _startTime;
}

float Clock::getElapsedAsSeconds() const {
    return getElapsed().asSeconds();
}

int32 Clock::getElapsedAsMilliseconds() const {
    return getElapsed().asMilliseconds();
}

int64 Clock::getElapsedAsMicroseconds() const {
    return getElapsed().asMicroseconds();
}

Time Clock::restart() {
    Time now = getTime();
    Time elapsed = now - _startTime;
    _startTime = now;

    return elapsed;
}

Time Clock::getStartTime() const {
    return _startTime;
}
