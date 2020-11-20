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

#include "player.h"

#include "input.h"
#include "graphics.h"

Player::Player():
    Actor(),
    _speed(400)
{
    setSize({20, 20});
}

void Player::act(float delta) {
    vec2f spd;
    
    if (isKeyDown(vk_left, vk_a)) {
        spd.x = -_speed * delta;
    } else if (isKeyDown(vk_right, vk_d)) {
        spd.x = _speed * delta;
    }
    
    if (isKeyDown(vk_down, vk_s)) {
        spd.y = _speed * delta;
    } else if (isKeyDown(vk_up, vk_w)) {
        spd.y = -_speed * delta;
    }
    
    move(spd);
}

void Player::draw() {
    drawRectangle(_position, _size, false);
}
