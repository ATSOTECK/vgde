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

#include "actor.h"
#include "stage.h"

Actor::Actor():
    _visible(true),
    _z(0.f),
    _stage(null)
{
    //
}

void Actor::setStage(Stage *stage) {
    _stage = stage;
}

bool Actor::visible() const {
    return _visible;
}

void Actor::setVisible(bool visible) {
    _visible = visible;
}

float Actor::z() const {
    return _z;
}

void Actor::setZIndex(float z) {
    _z = z;
    _stage->sort();
}

void Actor::toBack() {
    setZIndex(_stage->back() - 1.f);
}

void Actor::toFront() {
    setZIndex(_stage->front() + 1.f);
}

void Actor::ding(const String &name) {
    //
}

bool operator< (const Actor &lhs, const Actor &rhs) {
    return (lhs._z < rhs._z);
}
