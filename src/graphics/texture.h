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

    float width() const;
    float height() const;
    vec2f size() const;

    uint textureID() const;
    uint slot() const;

private:
    uint _textureID;
    uint _slot;
    std::string _path;
    uchar *_data;
    float _width;
    float _height;
    int _bpp;
};


#endif //__VGDE_TEXTURE_H__
