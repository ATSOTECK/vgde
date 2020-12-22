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

#include "alCall.h"

#include <iostream>

void alCheckErrors(const std::string &file, uint32 line) {
    ALenum error = alGetError();
    std::string desc;
    
    if (error != AL_NO_ERROR) {
        switch (error) {
            case AL_INVALID_NAME: {
                desc = "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            } break;
            
            case AL_INVALID_ENUM: {
                desc = "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            } break;
            
            case AL_INVALID_VALUE: {
                desc = "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            } break;
            
            case AL_INVALID_OPERATION: {
                desc = "AL_INVALID_OPERATION: the requested operation is not valid";
            } break;
            
            case AL_OUT_OF_MEMORY: {
                desc = "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            } break;
    
            default: {
                desc = "unknown al error: " + std::to_string(error);
            }
        }
        
        verr("on line " << line << " in " << file << std::endl << desc);
    }
}
