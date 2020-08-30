#ifndef __VGDE_FONT_H__
#define __VGDE_FONT_H__

#include "config.h"
#include "shader.h"
#include "vec.h"

#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H"freetype/freetype.h"

static const int DEFAULT_FONT_SIZE = 24;

struct Character {
    uint textureID;
    vec2i size;
    vec2i bearing;
    uint advance;
};

class Font {
public:
    explicit Font(const std::string &fnt);

    void draw(const std::string &txt, float x, float y, float scale, Shader *shader);
private:
    void *_lib;
    void *_face;
    uint _ibo;
    uint _vao;
    uint _vbo;
    std::map<char, Character> _chars;
    void loadFont(const std::string &filename);
};


#endif //__VGDE_FONT_H__
