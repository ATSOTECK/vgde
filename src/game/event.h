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

#ifndef __VGDE_EVENT_H__
#define __VGDE_EVENT_H__

#include "util/vcontainer.h"
#include "util/vstring.h"

#define EventCallback var *vcat(_tmpEvent,__COUNTER__) = new Event

class Actor;
class Screen;

struct EventTriggerInfo {
    //
};

class Event {
public:
    Event(const String &name, Actor *actor, bool active = true, bool repeat = false);
    Event(const String &name, Screen *screen, bool active = true, bool repeat = false);
    
    keep bool active() const;
    void active(bool active);
    
    keep bool shouldTrigger() const;
    
    keep bool repeat() const;
    
    keep Actor *actor() const;
    keep Screen *screen() const;
    keep String name() const;
    
    static const bool Repeat;
    static const bool Single;
    
private:
    String _name;
    Actor *_actor;
    Screen *_screen;
    bool _active;
    bool _repeat;
    
    LContainer _fnCall;
};


#endif //__VGDE_EVENT_H__
