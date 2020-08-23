#ifndef __VGDE_CLOCK_H__
#define __VGDE_CLOCK_H__

#include "vtime.h"

class Clock {
public:
    Clock();

    static Time getTime();
    static float getTimeAsSeconds();
    static int32 getTimeAsMilliseconds();
    static int64 getTimeAsMicroseconds();

    static int getHour(bool use12hr = false);
    static int getMinute();
    static int getSecond();
    static int getMillisecond();

    static bool isAM();
    static bool isPM();

    Time getElapsed() const;
    float getElapsedAsSeconds() const;
    int32 getElapsedAsMilliseconds() const;
    int64 getElapsedAsMicroseconds() const;

    Time restart();
    Time getStartTime() const;

private:
    Time _startTime;
};


#endif //__VGDE_CLOCK_H__
