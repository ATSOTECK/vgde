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

#include "clock.h";
#include "graphics/draw.h"

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
    for (var &pair : _textureMap) {
        if (pair.second.obj == texture) {
            if (pair.second.count == 1) {
                delete texture;
                _textureMap.erase(pair.first);

                return;
            } else {
                --pair.second.count;
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
    for (var &pair : _fontMap) {
        if (pair.second.obj == font) {
            if (pair.second.count == 1) {
                delete font;
                _fontMap.erase(pair.first);
                
                return;
            } else {
                --pair.second.count;
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

void ResourceManager::checkTimers() {
    if (_timers.empty()) {
        return;
    }
    
    Time time = Clock::time();
    for (var timer : _timers) {
        if (time > timer->endTime() && timer->ticking()) {
            timer->ding();
            
            if (!timer->repeat()) {
                var idx = std::find(_timers.begin(), _timers.end(), timer);
                if (idx != _timers.end()) {
                    _timers.erase(idx);
                }
                
                delete timer;
                timer = null;
            } else {
                timer->restart();
            }
        }
    }
}
