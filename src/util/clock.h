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
