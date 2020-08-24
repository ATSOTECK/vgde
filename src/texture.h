#ifndef __VGDE_TEXTURE_H__
#define __VGDE_TEXTURE_H__

#include <string>

#include "config.h"
#include "gl.h"
#include "vec.h"

class Texture {
public:
    explicit Texture(const std::string &path);
    ~Texture();

    void bind(uint slot = 0);
    void unbind() const;

    int width() const;
    int height() const;
    vec2i size() const;

    uint textureID() const;
    uint slot() const;

private:
    uint _textureID;
    uint _slot;
    std::string _path;
    uchar *_data;
    int _width;
    int _height;
    int _bpp;
};


#endif //__VGDE_TEXTURE_H__
