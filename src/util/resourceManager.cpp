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

#include "resourceManager.h"

#include "game/event.h"
#include "graphics/draw.h"
#include "timer.h"

ResourceManager *ResourceManager::_resourceManager = null;

ResourceManager::ResourceManager() {
    setImgPath("res/img");
    setFontPath("res/fnt");
}

ResourceManager *ResourceManager::instance() {
    if (_resourceManager == null) {
        _resourceManager = new ResourceManager();
    }

    return _resourceManager;
}

void ResourceManager::cleanUp() {
    for (var [s, r] : _textureMap) {
        delete r.obj;
    }
    
    for (var [s, r] : _fontMap) {
        delete r.obj;
    }
    
    for (var s : _shaders) {
        removeShader(s);
    }
    
    for (var t : _timers) {
        removeTimer(t);
    }
    
    for (var e : _events) {
        removeEvent(e);
    }
}

Texture *ResourceManager::loadTexture(const std::string &path) {
    Texture *texture = null;

    var idx = _textureMap.find(path);
    if (idx != _textureMap.end()) {
        texture = _textureMap[path].obj;
        ++_textureMap[path].count;
    } else {
        texture = new Texture(_imgPath + "/" + path);

        if (texture->width() == 0 || texture->height() == 0) {
            delete texture;
            texture = new Texture(path);

            if (texture->width() == 0 || texture->height() == 0) {
                delete texture;
                return null;
            }
        }

        _textureMap[path].obj = texture;
        _textureMap[path].count = 1;
    }

    return texture;
}

void ResourceManager::unloadTexture(Texture *texture) {
    for (var [s, r] : _textureMap) {
        if (r.obj == texture) {
            if (r.count <= 1) {
                delete texture;
                _textureMap.erase(s);

                return;
            } else {
                --r.count;
                return;
            }
        }
    }
}

Font *ResourceManager::loadFont(const std::string &path) {
    Font *font = null;
    var idx = _fontMap.find(path);
    if (idx != _fontMap.end()) {
        font = _fontMap[path].obj;
        ++_fontMap[path].count;
    } else {
        font = new Font(_fontPath + "/" + path);
        
        if (!font->loaded()) {
            delete font;
            font = new Font(path);
            
            if (!font->loaded()) {
                delete font;
                return null;
            }
        }
        
        _fontMap[path].obj = font;
        _fontMap[path].count = 1;
    }
    
    return font;
}

void ResourceManager::unloadFont(Font *font) {
    for (var [s, r] : _fontMap) {
        if (r.obj == font) {
            if (r.count <= 1) {
                delete font;
                _fontMap.erase(s);
                
                return;
            } else {
                --r.count;
                return;
            }
        }
    }
}

void ResourceManager::setResPath(const std::string &path) {
    _imgPath = path;
    _fontPath = path;
}

void ResourceManager::setImgPath(const std::string &path) {
    _imgPath = path;
}

void ResourceManager::setFontPath(const std::string &path) {
    _fontPath = path;
}

void ResourceManager::addShader(Shader *shader) {
    _shaders.push_back(shader);
}

void ResourceManager::removeShader(Shader *shader) {
    var idx = std::find(_shaders.begin(), _shaders.end(), shader);
    if (idx == _shaders.end()) {
        return;
    }

    _shaders.erase(idx);
    delete shader;
}

void ResourceManager::updateShaderProjections() {
    if (_shaders.empty()) {
        return;
    }

    glm::mat4 projection = drawGetProjection();

    for (var shader : _shaders) {
        shader->setMat4("projection", projection, true);
    }
}

void ResourceManager::addTimer(Timer *timer) {
    _timers.push_back(timer);
}

void ResourceManager::removeTimer(Timer *timer) {
    if (_timers.empty()) {
        return;
    }
    
    var idx = std::find(_timers.begin(), _timers.end(), timer);
    if (idx != _timers.end()) {
        _timers.erase(idx);
    
        delete timer;
        timer = null;
    }
}

void ResourceManager::checkTimers() {
    if (_timers.empty()) {
        return;
    }
    
    for (var timer : _timers) {
        if (timer->timeLeft() <= Time::Zero && timer->ticking()) {
            timer->ding();
            
            timer->repeat() ? timer->restart() : removeTimer(timer);
        }
    }
}

void ResourceManager::removeScreenTimersFor(Screen *screen) {
    if (_timers.empty()) {
        return;
    }
    
    for (var timer : _timers) {
        if (timer->screen() == screen) {
            removeTimer(timer);
        }
    }
}

void ResourceManager::removeLambdaTimers() {
    if (_timers.empty()) {
        return;
    }
    
    for (var timer : _timers) {
        if (timer->lambda()) {
            removeTimer(timer);
        }
    }
}

void ResourceManager::addEvent(Event *event) {
    _events.push_back(event);
}

void ResourceManager::removeEvent(Event *event) {
    if (_events.empty()) {
        return;
    }
    
    var idx = std::find(_events.begin(), _events.end(), event);
    if (idx != _events.end()) {
        _events.erase(idx);
        
        delete event;
        event = null;
    }
}

void ResourceManager::checkEvents() {
    if (_events.empty()) {
        return;
    }
    
    for (var event : _events) {
        if (event->active() && event->shouldTrigger()) {
            //send the event, save the sent event in the event log
            //that way if another event depends on this event it can
            //check the event log to see if it has been triggered
            //if it is a single shot event delete the event
        }
    }
}

void ResourceManager::removeScreenEventsFor(Screen *screen) {
    if (_events.empty()) {
        return;
    }
    
    for (var event : _events) {
        if (event->screen() == screen) {
            removeEvent(event);
        }
    }
}

void ResourceManager::removeLambdaEvents() {
    if (_events.empty()) {
        return;
    }
}
