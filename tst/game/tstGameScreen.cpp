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

#include "input.h"
#include "util/humanReadable.h"
#include "util/resourceManager.h"

TstGameScreen::TstGameScreen():
    Screen("game"),
    _vgde(VGDE::instance()),
    _chinese(null),
    _tv(null)
{
    //
}

TstGameScreen::~TstGameScreen() {
    ResourceManager::instance()->unloadFont(_chinese);
}

void TstGameScreen::show() {
    _chinese = ResourceManager::instance()->loadFont("SimSun.ttf");
    
    _tv = new Sprite("tv.png");
    _tv->setOrigin(_tv->center());
    _tv->setPosition(_tv->center());
}

void TstGameScreen::hide() {
    //
}

void TstGameScreen::render(float delta) {
    if (isKeyDown(vk_escape)) {
        _vgde->exit();
    }
    
    _tv->rotate(-50.f * delta);
    _tv->draw();
    
    std::string txt = "fps: " + std::to_string(_vgde->fps()) + "\t[blue]" + std::to_string(_vgde->frameTime()) + "ms\n"
                      + "[cyan]" + secondsToHHMMSS((int)_vgde->totalInGameTime());
    drawText(txt, 0, 0, 1, Color::Green);
    
    //drawText("The quick brown fox jumped over the lazy dog [green]:[])", 10, 10, 1, Color::White);
    drawText("我当然还是[red]爱[]你", 800, 10, 1, Color::White, _chinese);
    
    float mx = mouseX();
    float my = mouseY();
    drawCircle(mx, my, 25);
    drawArc(mx, my, 35, 0, 90);
    drawArc(mx, my, 45, 90, 180);
    drawArc(mx, my, 35, 180, 270);
    drawArc(mx, my, 45, 270, 360);
}

void TstGameScreen::resize(const vec2f &size) {
    //
}