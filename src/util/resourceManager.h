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

#ifndef __VGDE_RESOURCEMANAGER_H__
#define __VGDE_RESOURCEMANAGER_H__

#include "game/screen.h"
#include "graphics/font.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

#include <al.h>
#include <map>
#include <string>
#include <vector>

template <typename T>
struct ref {
    T obj;
    int count;
};

class Event;
class Timer;

class ResourceManager {
public:
    static ResourceManager *instance();
    
    void cleanUp();

    Texture *loadTexture(const std::string &path);
    void unloadTexture(Texture *texture);

    Font *loadFont(const std::string &font);
    void unloadFont(Font *font);
    keep Font *defaultFont() const;
    
    void setResPath(const std::string &path);
    void setImgPath(const std::string &path);
    void setFontPath(const std::string &path);

    void addShader(Shader *shader);
    void removeShader(Shader *shader);
    void updateShaderProjections();
    
    void addTimer(Timer *timer);
    void removeTimer(Timer *timer);
    void checkTimers();
    void removeScreenTimersFor(Screen *screen);
    void removeLambdaTimers();
    
    void addEvent(Event *event);
    void removeEvent(Event *event);
    void checkEvents();
    void removeScreenEventsFor(Screen *screen);
    void removeLambdaEvents();
    
    //Sound *loadSound(const String &path);
    
private:
    ResourceManager();
    static ResourceManager *_resourceManager;

    std::map<std::string, ref<Texture*> > _textureMap;
    std::vector<Shader*> _shaders;
    std::map<std::string, ref<Font*> > _fontMap;
    std::vector<Timer *> _timers;
    std::vector<Event *> _events;

    std::string _imgPath;
    std::string _fontPath;
    Font *_defaultFont;
};


#endif //__VGDE_RESOURCEMANAGER_H__
