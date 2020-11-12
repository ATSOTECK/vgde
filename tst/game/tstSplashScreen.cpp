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

#include "tstSplashScreen.h"

#include "input.h"
#include "util/timer.h"

TstSplashScreen::TstSplashScreen():
    Screen("splash"),
    _vgde(VGDE::instance()),
    _splash("birb.png"),
    _chinese(null),
    _txtx(0.f),
    _txty(0.f)
{
    //
}

TstSplashScreen::~TstSplashScreen() {
    ResourceManager::instance()->unloadFont(_chinese);
}

void TstSplashScreen::show() {
    if (initialized()) {
        return;
    }
    
    mouseHide();
    
    _splash.setOrigin(_splash.center());
    _splash.setScale(.75f);
    _splash.setPosition(_vgde->windowCenter());
    
    _chinese = ResourceManager::instance()->loadFont("SimSun.ttf");
    
    if (Clock::isPM()) {
        db(Clock::hour(true) << ":" << Clock::minute() << ":" << Clock::second() << ":" << Clock::millisecond() << " pm");
    } else {
        db(Clock::hour(true) << ":" << Clock::minute() << ":" << Clock::second() << ":" << Clock::millisecond() << " am");
    }
    
    var hiTimer = new Timer(this, "hi", Time::seconds(1), Timer::Repeat);
    hiTimer->start();
    
    TimerCallback(Time::seconds(1), Timer::Repeat, [=]{
        db("test from old screen");
    });
    
    var timer = new Timer(this, Time::seconds(5), Timer::Repeat);
    timer->start();
}

void TstSplashScreen::hide() {
    //
}

void TstSplashScreen::render(float delta) {
    if (!isKeyDown(vk_alt) && isKeyDown(vk_any)) {
        //_vgde->screenGoto("game", true);
        _vgde->screenGotoNext(true);
        return;
    }
    
    _splash.draw();
    
    drawText("我当然还是[red]爱[]你", _txtx, _txty, 1, Color::White, _chinese);
    drawText("Of Course I Still [red]Love[] You", _txtx - 100, _txty + 330, 1, Color::White);
    //float cx = _vgde->windowCenter().x;
    //float wh = _vgde->windowHeight();
    //drawLine(cx, 0, cx, wh);
}

void TstSplashScreen::resize(const vec2f &size) {
    _txtx = _vgde->windowCenter().x - 85;
    _txty = _vgde->windowCenter().y - 180;
    _splash.setPosition(_vgde->windowCenter());
}

void TstSplashScreen::ding(const String &name) {
    if (name == "hi") {
        db("hi from splash screen");
    } else if (active()) {
        _vgde->screenGoto("game", false);
    }
}
