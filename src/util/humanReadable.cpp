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

#include "humanReadable.h"

#include "config.h"
#include <iostream>

std::string secondsToHHMMSS(int seconds) {
    //TODO("Skyler","Formatting.");
    
    int h = seconds / 3600;
    int m = (seconds - h * 3600) / 60;
    int s = (seconds - h * 3600) - m * 60;

    return std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s);
}
