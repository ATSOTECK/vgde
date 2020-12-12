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

#ifndef __VGDE_STAGE_H__
#define __VGDE_STAGE_H__

#include "actor.h"

#include <vector>

class Stage {
public:
    explicit Stage(uint32 maxActors = 1000);
    
    void sort();
    
    bool addActor(Actor *actor);
    
    void act(float delta);
    void draw();
    
    keep float front() const;
    keep float back() const;
    
    keep uint32 maxActors() const;
    void setMaxActors(uint32 max);
    
    void reserve(uint32 count);
    
private:
    std::vector<Actor *> _actors;
    uint32 _maxActors;
};


#endif //__VGDE_STAGE_H__
