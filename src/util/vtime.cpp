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

Time seconds(float seconds) {
    return Time(seconds * 1000000);
}

Time milliseconds(int32 milliseconds) {
    return Time(milliseconds * 1000);
}

Time microseconds(int64 microseconds) {
    return Time(microseconds);
}

Time operator -(const Time &left, const Time &right) {
    return microseconds(left.asMicroseconds() - right.asMicroseconds());
}
