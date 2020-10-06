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
    ~Font();

    //TODO(Skyler): Make this work with other characters outside ascii.
    void draw(const String &txt, float x, float y, float scale, Shader *shader, const Color &color);
private:
    void getGlyph(uint codePoint, int size, bool bold = false, float outlineThickness = 0);

    void *_lib;
    void *_face;
    uint _ibo;
    uint _vao;
    uint _vbo;
    std::map<uint, Character> _chars;
    void loadFont(const std::string &filename);
};


#endif //__VGDE_FONT_H__
