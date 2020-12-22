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

#include "event.h"

#include "actor.h"
#include "screen.h"

const bool Event::Repeat = true;
const bool Event::Single = false;

Event::Event(const String &name, Actor *actor, bool active, bool repeat):
    _name(name),
    _actor(actor),
    _screen(null),
    _active(active),
    _repeat(repeat)
{
    //
}

Event::Event(const String &name, Screen *screen, bool active, bool repeat):
    _name(name),
    _actor(null),
    _screen(screen),
    _active(active),
    _repeat(repeat)
{
    //
}

bool Event::active() const {
    return _active;
}

void Event::active(bool active) {
    _active = active;
}

bool Event::shouldTrigger() const {
    return false;
}

bool Event::repeat() const {
    return _repeat;
}

Actor *Event::actor() const {
    return _actor;
}

Screen *Event::screen() const {
    return _screen;
}

String Event::name() const {
    return _name;
}
