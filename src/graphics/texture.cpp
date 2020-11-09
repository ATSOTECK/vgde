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

#include "texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Texture::Texture(const vec2f &size) :
    _textureID(0),
    _slot(0),
    _width(0),
    _height(0),
    _bpp(4)
{
    resize(size);
}

Texture::Texture(const std::string &path) :
        _textureID(0),
        _slot(0),
        _path(path),
        _width(0),
        _height(0),
        _bpp(0)
{
    stbi_set_flip_vertically_on_load(1);
    int w = 0, h = 0;
    uchar *data = stbi_load(_path.c_str(), &w, &h, &_bpp, 4);
    _width = (float)w;
    _height = (float)h;

    if (data == null) {
        vgdewarn("Could not load " << path);
        return;
    }

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    unbind();

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &_textureID);
}

void Texture::bind(uint slot) {
    _slot = slot;
    glActiveTexture(GL_TEXTURE0 + _slot);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::clear() const {
    glClearTexImage(_textureID, 0, GL_RGBA, GL_UNSIGNED_BYTE, null);
}

float Texture::width() const {
    return _width;
}

float Texture::height() const {
    return _height;
}

vec2f Texture::size() const {
    return {_width, _height};
}

void Texture::resize(const vec2f &size) {
    _width = size.x;
    _height = size.y;
    
    if (_textureID == 0) {
        glGenTextures(1, &_textureID);
    }
    glBindTexture(GL_TEXTURE_2D, _textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, null);
    unbind();
}

uint Texture::textureID() const {
    return _textureID;
}

uint Texture::slot() const {
    return _slot;
}

bool Texture::saveToFile(const std::string &filename) const {
    int w = (int)_width;
    int h = (int)_height;
    GLsizei size = w * h * 4u;
    var data = new uint8[size];
    var pixels = new uint8[size];
    
    glGetTextureImage(_textureID, 0, GL_RGBA, GL_UNSIGNED_BYTE, size, data);
    
    for (int y = 0; y < h; ++y) {
        uint8 *ptr = &data[(h - y - 1) * w * 4];
        memcpy_s(&pixels[y * w * 4], w * 4, ptr, w * 4);
    }
    
    //TODO(Skyler): Move to thread.
    bool success = (stbi_write_png(filename.c_str(), w, h, 4, pixels, 0) != 0);
    
    delete[] data;
    delete[] pixels;
    
    return success;
}
