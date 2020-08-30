#include "texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &path) :
        _textureID(0),
        _slot(0),
        _path(path),
        _data(null),
        _width(0),
        _height(0),
        _bpp(0)
{
    stbi_set_flip_vertically_on_load(1);
    int w = 0, h = 0;
    _data = stbi_load(_path.c_str(), &w, &h, &_bpp, 4);
    _width = (float)w;
    _height = (float)h;

    if (_data == null) {
        vgderr("Could not load " << path);
        return;
    }

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
    bind();

    if (_data != null) {
        stbi_image_free(_data);
    }
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

float Texture::width() const {
    return _width;
}

float Texture::height() const {
    return _height;
}

vec2f Texture::size() const {
    return {_width, _height};
}

uint Texture::textureID() const {
    return _textureID;
}

uint Texture::slot() const {
    return _slot;
}
