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

#include "tstGameScreen.h"

#include "graphics/draw.h"
#include "input.h"

TstGameScreen::TstGameScreen():
    Screen("game"),
    _vgde(VGDE::instance())
{
    //
}

TstGameScreen::~TstGameScreen() = default;

void TstGameScreen::show() {
    //
}

void TstGameScreen::hide() {
    //
}

void TstGameScreen::render(float delta) {
    if (isKeyDown(vk_escape)) {
        _vgde->exit();
    }
    
    drawText("game [red]:[])", 10, 10, 1, Color::White);
}
