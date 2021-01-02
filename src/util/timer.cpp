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

#include "timer.h"

#include "clock.h"

const bool Timer::Repeat = true;
const bool Timer::Single = false;

Timer::Timer(Actor *actor, Time time, bool repeat) :
    _actor(actor),
    _screen(null),
    _name(""),
    _time(time),
    _repeat(repeat),
    _ticking(false),
    _startTime(Time::Zero),
    _dingCount(0),
    _lambda(false)
{
    ResourceManager::instance()->addTimer(this);
}

Timer::Timer(Actor *actor, const String &name, Time time, bool repeat) :
    _actor(actor),
    _screen(null),
    _name(name),
    _time(time),
    _repeat(repeat),
    _ticking(false),
    _startTime(Time::Zero),
    _dingCount(0),
    _lambda(false)
{
    ResourceManager::instance()->addTimer(this);
}

Timer::Timer(Screen *screen, Time time, bool repeat) :
    _actor(null),
    _screen(screen),
    _name(""),
    _time(time),
    _repeat(repeat),
    _ticking(false),
    _startTime(Time::Zero),
    _dingCount(0),
    _lambda(false)
{
    ResourceManager::instance()->addTimer(this);
}

Timer::Timer(Screen *screen, const String &name, Time time, bool repeat) :
    _actor(null),
    _screen(screen),
    _name(name),
    _time(time),
    _repeat(repeat),
    _ticking(false),
    _startTime(Time::Zero),
    _dingCount(0),
    _lambda(false)
{
    ResourceManager::instance()->addTimer(this);
}

void Timer::start() {
    _ticking = true;
    _startTime = Clock::time();
}

void Timer::restart(const Time &time) {
    if (time != Time::Zero && time != _time) {
        _time = time;
        _dingCount = 0;
    }
    
    start();
}

void Timer::stop() {
    _ticking = false;
    _startTime = Time::Zero;
}

void Timer::ding() {
    if (_lambda) {
        _fnCall();
    } else if (_actor == null && _screen == null) {
        _startTime = Time::Zero;
        _time = Time::Zero;
        _repeat = false;
        
        return;
    } else {
        _actor != null ? _actor->ding(_name) : _screen->ding(_name);
    }
    
    ++_dingCount;
}

bool Timer::repeat() const {
    return _repeat;
}

bool Timer::ticking() const {
    return _ticking;
}

Time Timer::startTime() const {
    return _startTime;
}

Time Timer::endTime() const {
    return _startTime + _time;
}

Time Timer::timeLeft() const {
    return (_startTime + _time) - Clock::time();
}

int Timer::dingCount() const {
    return _dingCount;
}

bool Timer::lambda() const {
    return _lambda;
}

Actor *Timer::actor() const {
    return _actor;
}

Screen *Timer::screen() const {
    return _screen;
}

String Timer::name() const {
    return _name;
}
