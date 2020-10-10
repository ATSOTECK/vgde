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

#ifndef __VGDE_SCREEN_H__
#define __VGDE_SCREEN_H__

#include "util/vstring.h"
#include "vec.h"

class Screen {
public:
    explicit Screen(const String &name);
    virtual ~Screen() = 0;
    
    virtual void show() = 0;
    virtual void hide() = 0;
    
    void pause();
    void resume();
    bool paused() const;
    
    //void resize(const vec2f &size);
    
    virtual void render(float delta) = 0;
    
    virtual void resize(const vec2f &size) = 0;
    
    String name() const;
    
private:
    bool _paused;
    String _name;
};


#endif //__VGDE_SCREEN_H__
