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

#include "tstGameScreen.h"

#include "input.h"
#include "player.h"
#include "util/humanReadable.h"
#include "util/timer.h"
#include "util/vmath.h"

TstGameScreen::TstGameScreen():
    Screen("game"),
    _vgde(VGDE::instance()),
    _chinese(null),
    _tv(null),
    _t(null),
    _stage(new Stage())
{
    //
}

TstGameScreen::~TstGameScreen() {
    ResourceManager::instance()->unloadFont(_chinese);
}

void TstGameScreen::show() {
    if (initialized()) {
        return;
    }
    
    _chinese = ResourceManager::instance()->loadFont("SimSun.ttf");
    
    _text = new Text("我当然还是[red]爱[]你", _chinese);
    _text->setPosition({100, 100});
    _text->setColor(choose(Color::White, Color::Red, Color::Blue, Color::Black, Color::random()));
    
    _tv = new Sprite("tv.png");
    _tv->setOrigin(_tv->center());
    _tv->setPosition(_tv->center());
    
    _sprs.reserve(500);
    for (int i = 0; i < 500; ++i) {
        var fire = new Sprite("fire.png");
        fire->setSize({32, 32});
        fire->setPosition(randomPosition(700, 500));
        fire->setOrigin(fire->center());
        fire->setRotation(random(360));
        _sprs.push_back(fire);
    }
    
    _t = new RenderTexture({200, 600});
    _t->bind();
    //_t->clear();
    drawText("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\nEEEEE\nEEEEEEEEQEE\nEEEEEE\nEEEEE\nEEEEEEEEEE", 0, 0, 1, Color::White);
    _t->unbind();
    
    //_t->texture()->saveToFile("_t.png");
    
    //var timer = new Timer(this, Time::seconds(10), Timer::Repeat);
    //timer->start();
    
    /*
    TimerCallback(Time::seconds(2), Timer::Repeat, [=]{
        db(_tv->rotation());
        db("yo from timer");
    });
    
    TimerCallback(Time::seconds(4), Timer::Repeat, [=]{
        db(_t->position().x);
        db("ye from timer");
    });
    */
    
    //drawSetFontSize(12);
    //_chinese->setSize(6);
    
    _stage->addActor(new Player());
}

void TstGameScreen::hide() {
    //
}

void TstGameScreen::render(float delta) {
    if (isKeyDown(vk_escape)) {
        _vgde->exit();
    }
    
    _t->setPosition(mousePosition());
    
    if (!paused()) {
        _tv->rotate(-50.f * delta);
    }
    _tv->draw();
    /*
    for (var s : _sprs) {
        s->rotate(random(2));
        s->setScale(random(2) / 8);
        s->setOrigin(s->center());
        //s->draw();
    }*/
    
    std::string txt = "fps: " + std::to_string(_vgde->fps()) + "\t[blue]" + std::to_string(_vgde->frameTimeMS()) + "ms\n"
                      + "[cyan]" + secondsToHHMMSS((int)_vgde->totalInGameTime());
    drawText(txt, 0, 0, 1, Color::Green);
    
    float x = drawText("The quick brown fox jumped over the lazy dog [green]:[])", 10, 550, 1, Color::White);
    drawText("我当然还是[red]爱[]你", 800, 10, 1, Color::White, _chinese);
    
    drawLine(x, 0, x, 800);
    
    _t->draw();
    _text->draw();
    
    _stage->act(delta);
    _stage->draw();
    
    float mx = mouseX();
    float my = mouseY();
    drawCircle(mx, my, 25);
    drawArc(mx, my, 35, 0, 90);
    drawArc(mx, my, 45, 90, 180);
    drawArc(mx, my, 35, 180, 270);
    drawArc(mx, my, 45, 270, 360);
    
    if (isKeyPressed(vk_f)) {
        _vgde->debug(delta);
    }
    
    if (isKeyPressed(vk_f12)) {
        _vgde->screenshot();
    }
    
    if (isKeyPressed(vk_l)) {
        _vgde->windowSetVSync(false);
    }
    
    if (isKeyPressed(vk_k)) {
        _vgde->windowSetVSync(true);
    }
    
    if (isKeyPressed(vk_e)) {
        _vgde->vError("yeet");
    }
    
    if (isKeyPressed(vk_q)) {
        _vgde->vInfo("yeet");
        _vgde->vWarn("yeet");
        _vgde->vDebug("yeet");
        _vgde->vInfo("yeeting");
    
        _vgde->info("yeet");
        _vgde->warn("yeet");
        _vgde->debug("yeet");
        _vgde->info("yeeting");
    }
}

void TstGameScreen::resize(const vec2f &size) {
    //
}

void TstGameScreen::ding(const String &name) {
    if (active()) {
        _vgde->screenGotoPrevious();
    }
}
