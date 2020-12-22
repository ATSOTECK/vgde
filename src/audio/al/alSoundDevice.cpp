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

#include "alSoundDevice.h"

#include "alCall.h"
#include "config.h"

#include <iostream>

AlSoundDevice *AlSoundDevice::_instance = null;

AlSoundDevice::AlSoundDevice() {
    //TODO(Skyler): Have the ability to choose the sound device to use?
    _device = alCall(alcOpenDevice(null)); //Default sound device.
    if (_device == null) {
        vgderr("Failed to get sound device!");
    }
    
    _context = alCall(alcCreateContext(_device, null));
    if (_context == null) {
        vgderr("Failed to set sound context!");
    }
    
    if (!alcMakeContextCurrent(_context)) {
        vgderr("Failed to make the context current!");
    }
    
    const ALCchar *name = null;
    if (alcIsExtensionPresent(_device, "ALC_ENUMERATE_ALL_EXT")) {
        name = alcGetString(_device, ALC_ALL_DEVICES_SPECIFIER);
    } else if (name == null || alcGetError(_device) != AL_NO_ERROR) {
        name = alcGetString(_device, ALC_DEVICE_SPECIFIER);
    }
    
    db("Opened '" << name << "'");
}

AlSoundDevice *AlSoundDevice::instance() {
    if (_instance == null) {
        _instance = new AlSoundDevice();
    }
    
    return _instance;
}
