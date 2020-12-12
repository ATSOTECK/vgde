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

#include "stage.h"

#include <algorithm>

Stage::Stage(uint32 maxActors) {
    setMaxActors(maxActors);
}

void Stage::sort() {
    std::sort(_actors.begin(), _actors.end());
}

bool Stage::addActor(Actor *actor) {
    if (actor == null || _actors.size() > _maxActors) {
        return false;
    }
    
    actor->setStage(this);
    _actors.push_back(actor);
    sort();
    
    return true;
}

void Stage::act(float delta) {
    for (var a : _actors) {
        a->act(delta);
    }
}

void Stage::draw() {
    for (var a : _actors) {
        a->draw();
    }
}

float Stage::front() const {
    if (_actors.empty()) {
        return 0.f;
    }
    
    return _actors[_actors.size() - 1]->z();
}

float Stage::back() const {
    if (_actors.empty()) {
        return 0.f;
    }
    
    return _actors[0]->z();
}

uint32 Stage::maxActors() const {
    return _maxActors;
}

void Stage::setMaxActors(uint32 max) {
    _maxActors = max;
    
    if (_maxActors < 100) {
        _maxActors = 100;
    }
}

void Stage::reserve(uint32 count) {
    _actors.reserve(count);
}
