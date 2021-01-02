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

#include "alSoundBuffer.h"

#include <alext.h>
#include <sndfile.h>

ALuint AlSoundBuffer::loadSound(const String &path) {
    SF_INFO info;
    SNDFILE *file = sf_open(path.c_str(), SFM_READ, &info);
    
    if (file == null) {
        vgderr("Could not open sound '" << path << "', " << sf_strerror(file));
        return 0;
    }
    
    if (info.frames < 1 || info.frames > (sf_count_t)(INT_MAX / sizeof(short)) / info.channels) {
        sf_close(file);
        vgderr("Bad sample count in '" << path << "' (" << info.frames << ")");
        return 0;
    }
    
    ALenum format = AL_NONE;
    bool cmd = sf_command(file, SFC_WAVEX_GET_AMBISONIC, null, 0) == SF_AMBISONIC_B_FORMAT;
    
    if (info.channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (info.channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else if (info.channels == 3 && cmd) {
        format = AL_FORMAT_BFORMAT2D_16;
    } else if (info.channels == 4 && cmd) {
        format = AL_FORMAT_BFORMAT3D_16;
    }
    
    if (format == AL_NONE) {
        sf_close(file);
        vgderr("Unsupported channel count: " << info.channels);
        return 0;
    }
    
    var buf = new short[(size_t)(info.frames * info.channels) * sizeof(short)];
    sf_count_t frameCount = sf_readf_short(file, buf, info.frames);
    
    if (frameCount < 1) {
        delete[] buf;
        sf_close(file);
        vgderr("Failed to read samples in '" << path << "' (" << frameCount << ")");
        return 0;
    }
    
    ALsizei size = (ALsizei)(frameCount * info.channels) * (ALsizei)sizeof(short);
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, buf, size, info.samplerate);
    
    delete[] buf;
    sf_close(file);
    
    
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        vgderr("OpenAL error: " << alGetString(error));
        if (buffer && alIsBuffer(buffer)) {
            alDeleteBuffers(1, &buffer);
        }
        
        return 0;
    }
    
    _buffers.push_back(buffer);
    
    return buffer;
}

bool AlSoundBuffer::deleteSound(ALuint buffer) {
    var idx = std::find(_buffers.begin(), _buffers.end(), buffer);
    if (idx != _buffers.end()) {
        alDeleteBuffers(1, &buffer);
        _buffers.erase(idx);
        
        return true;
    }
    
    return false;
}

AlSoundBuffer::AlSoundBuffer() = default;

AlSoundBuffer::~AlSoundBuffer() {
    alDeleteBuffers(_buffers.size(), _buffers.data());
    _buffers.clear();
}
