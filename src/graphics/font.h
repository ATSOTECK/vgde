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

#ifndef __VGDE_FONT_H__
#define __VGDE_FONT_H__

#include "color.h"
#include "config.h"
#include "shader.h"
#include "vec.h"
#include "util/vstring.h"

#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H//"freetype/freetype.h"

static const int DEFAULT_FONT_SIZE = 24;

struct Character {
    uint textureID{};
    vec2i size;
    vec2i bearing;
    uint advance{};
};

class Font {
public:
    explicit Font(const std::string &fnt);
    ~Font();
    
    void setSize(int size);
    keep int size() const;
    
    float draw(const String &txt, float x, float y, float scale, Shader *shader, const Color &color);
    
    keep uint ibo() const;
    keep uint vbo() const;
    
    keep bool loaded() const;
    void getGlyph(uint codePoint, int size, bool bold = false, float outlineThickness = 0);
    Character getChar(int size, uchar32 cp);
private:
    
    FT_Library _lib;
    FT_Face _face;
    uint _ibo;
    uint _vao;
    uint _vbo;
    std::map<int, std::map<uchar32, Character> > _charMap;
    void loadFont(const std::string &filename);
    
    bool _loaded;
    int _size;
};


#endif //__VGDE_FONT_H__
