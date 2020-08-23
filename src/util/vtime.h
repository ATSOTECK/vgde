#ifndef __VGDE_TIME_H__
#define __VGDE_TIME_H__

#include "config.h"

class Time {
public:
    Time();

    float asSeconds() const;
    int32 asMilliseconds() const;
    int64 asMicroseconds() const;

    static const Time Zero;
private:
    friend Time seconds(float);
    friend Time milliseconds(int32);
    friend Time microseconds(int64);

    explicit Time(int64 microseconds);

    int64 _microseconds;
};

Time seconds(float seconds);
Time milliseconds(int32 milliseconds);
Time microseconds(int64 microseconds);

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
