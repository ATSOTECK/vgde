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
    return microseconds(-right.asMicroseconds());
}
Time operator +(const Time &left, const Time &right) {
    return microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time &operator+=(Time &left, const Time &right) {
    return left = left + right;
}

Time operator -(const Time &left, const Time &right) {
    return microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time &operator-=(Time &left, const Time &right) {
    return left = left - right;
}

Time operator *(const Time &left, float right) {
    return seconds(left.asSeconds() * right);
}

Time operator *(const Time &left, int64 right) {
    return microseconds(left.asMicroseconds() * right);
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
    return seconds(left.asSeconds() / right);
}

Time operator /(const Time &left, int64 right) {
    return microseconds(left.asMicroseconds() / right);
}

Time &operator/=(Time &left, float right) {
    return left = left / right;
}

Time &operator/=(Time &left, int64 right) {
    return left = left / right;
}

Time operator %(const Time &left, const Time &right) {
    return microseconds(left.asMicroseconds() % right.asMicroseconds());
}
Time &operator%=(Time &left, const Time &right) {
    return left = left % right;
}
