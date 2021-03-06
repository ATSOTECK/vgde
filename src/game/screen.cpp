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

#include "screen.h"

Screen::Screen(const String &name):
    _paused(false),
    _name(name),
    _initialized(false),
    _active(false)
{
    //
}

Screen::~Screen() = default;

void Screen::pause() {
    _paused = true;
}

void Screen::resume() {
    _paused = false;
}

bool Screen::paused() const {
    return _paused;
}

void Screen::initialize() {
    _initialized = true;
}

bool Screen::initialized() const {
    return _initialized;
}

void Screen::setActive(bool active) {
    _active = active;
}

bool Screen::active() const {
    return _active;
}

String Screen::name() const {
    return _name;
}

void Screen::ding(const String &name) {
    // dong
}
