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

#ifndef __VGDE_TIMER_H__
#define __VGDE_TIMER_H__

#include "game/actor.h"
#include "game/screen.h"
#include "vstring.h"
#include "vtime.h"

#include <thread>

/*
template <int...>
struct seq {};

template <int N, int ...S>
struct gens : gens<N - 1, N, -1, S...> {};

template <int ...S>
struct gens<0, S...> {
    typedef seq<S...> type;
};

template <class Fn, class ...Args>
struct LContainer {
    Fn func;
    std::tuple<Args...> args;
    
    void call() {
        std::apply(func, args);
        callFunc(typename gens<sizeof...(Args)>::type());
    }
    
    template <int ...S>
    void callFunc(seq<S...>) {
        func(std::get<S>(args)...);
    }
};
*/

class Timer {
public:
    explicit Timer(Actor *actor, Time time, bool repeat = false);
    explicit Timer(Actor *actor, const String &name, Time time, bool repeat = false);
    explicit Timer(Screen *screen, Time time, bool repeat = false);
    explicit Timer(Screen *screen, const String &name, Time time, bool repeat = false);
    
    template <class Fn, class ...Args>
    explicit Timer(Time time, bool repeat, Fn &&fn, Args &&...args):
        _actor(null),
        _screen(null),
        _name(),
        _time(time),
        _repeat(repeat),
        _ticking(false),
        _startTime(Time::Zero),
        _dingCount(0),
        _lambda(false)
    {
        std::thread([=]{
            do {
                std::this_thread::sleep_for(std::chrono::microseconds(time.asMicroseconds()));
                std::invoke(fn, args...);
            } while (_repeat);
        }).detach();
    }
    
    void start();
    void restart(const Time &time = Time::Zero);
    void stop();
    void ding();
    
    keep bool repeat() const;
    keep bool ticking() const;
    keep Time startTime() const;
    keep Time endTime() const;
    keep Time timeLeft() const;
    keep int dingCount() const;
    
    keep Actor *actor() const;
    keep Screen *screen() const;
    
    static const bool Repeat;
    static const bool Single;
    
private:
    Actor *_actor;
    Screen *_screen;
    String _name;
    Time _time;
    bool _repeat;
    bool _ticking;
    Time _startTime;
    int _dingCount;
    bool _lambda;
};


#endif //__VGDE_TIMER_H__
