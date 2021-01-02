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

#include "font.h"

#include "gl.h"

#include <iostream>

Font::Font(const std::string &fnt) {
    loadFont(fnt);
}

Font::~Font() {
    if (_face) {
        FT_Done_Face((FT_Face)_face);
    }
    
    if (_lib) {
        FT_Done_FreeType((FT_Library)_lib);
    }
}

void Font::loadFont(const std::string &filename) {
    _lib = null;
    _face = null;
    _loaded = false;
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        vgderr("Could not init freetype!");
        return;
    }
    _lib = ft;

    FT_Face face;
    if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
        vgderr("Failed to load font!");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, DEFAULT_FONT_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        vgderr("Unable to load font \"" << filename << "\" failed to set the unicode charset.\n");
        FT_Done_Face(face);
        return;
    }
    
    _face = face;
    
    _size = 24;
    auto chars = std::map<uchar32, Character>();
    _charMap.insert_or_assign(_size, chars);
    
    for (uint c = 0; c < 128; ++c) {
        getGlyph(c, _size);
    }

    //FT_Done_Face(face);
    //FT_Done_FreeType(ft);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    glBindTexture(GL_TEXTURE_2D, 0);
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, null, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    _loaded = true;
}

void Font::setSize(int size) {
    _size = size;
    auto idx = _charMap.find(size);
    if (idx != _charMap.end()) {
        //_chars = _charMap[_size];
    } else {
        auto chars = std::map<uchar32, Character>();
        _charMap.insert_or_assign(_size, chars);
    }
}

int Font::size() const {
    return _size;
}

float Font::draw(const String &txt, float x, float y, float scale, Shader *shader, const Color &color) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);
    float ox = x;
    float len = 0.f;

    for (int i = 0; i < txt.length(); ++i) {
        uint32 cp = txt[i];
        
        if (cp == '\n') {
            y += (float)_size;
            x = ox;
            continue;
        }

        if (cp == '\t') {
            x += ((_charMap[_size][' '].advance >> 6u) * scale) * 4;
            continue;
        }

        if (cp == '[' && txt[i - 1] != '\\') {
            String str;
            while (cp != ']') {
                cp = txt[++i];
                if (cp != ']') {
                    str += cp;
                }
            }
            
            if (str.empty()) {
                shader->setVec3f("textColor", color.vec3gl());
            } else if (str == "black") {
                shader->setVec3f("textColor", Color::Black.vec3gl());
            } else if (str == "white") {
                shader->setVec3f("textColor", Color::White.vec3gl());
            } else if (str == "red") {
                shader->setVec3f("textColor", Color::Red.vec3gl());
            } else if (str == "green") {
                shader->setVec3f("textColor", Color::Green.vec3gl());
            } else if (str == "blue") {
                shader->setVec3f("textColor", Color::Blue.vec3gl());
            } else if (str == "lightBlue") {
                shader->setVec3f("textColor", Color::LightBlue.vec3gl());
            } else if (str == "yellow") {
                shader->setVec3f("textColor", Color::Yellow.vec3gl());
            } else if (str == "magenta") {
                shader->setVec3f("textColor", Color::Magenta.vec3gl());
            } else if (str == "cyan") {
                shader->setVec3f("textColor", Color::Cyan.vec3gl());
            } else if (str == "turquoise") {
                shader->setVec3f("textColor", Color::Turquoise.vec3gl());
            }
            
            continue;
        }
        
        Character ch = _charMap[_size][cp];
        if (ch.textureID == 0) {
            getGlyph(cp, _size);

            ch = _charMap[_size][cp];
        }

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.bearing.y * scale) + (float)_size;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        
        float verts[] = {
                //  Position         Texcoords
                xpos,      ypos + h, 0.0f, 1.0f, // Top-left
                xpos + w,  ypos + h, 1.0f, 1.0f, // Top-right
                xpos + w,  ypos,     1.0f, 0.0f, // Bottom-right
                xpos,      ypos,     0.0f, 0.0f  // Bottom-left
        };

        //Wrong but looks kinda cool
        /*
        float verts[] = {
                //  Position         Texcoords
                xpos,      ypos + h, 1.0f, 1.0f, // Top-left
                xpos + w,  ypos + h, 0.0f, 1.0f, // Top-right
                xpos + w,  ypos,     1.0f, 0.0f, // Bottom-right
                xpos,      ypos,     0.0f, 0.0f  // Bottom-left
        };//*/

        uint indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), verts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(sizeof(float) * 2));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
        x += (ch.advance >> 6u) * scale;
        
        if (x > len) {
            len = x;
        }
    }
    
    return len;
}

bool Font::loaded() const {
    return _loaded;
}

void Font::getGlyph(uint codePoint, int size, bool bold, float outlineThickness) {
    FT_Face face = (FT_Face)_face;
    
    //auto cp = FT_Get_Char_Index(face, codePoint);
    
    if (FT_Set_Pixel_Sizes(face, 0, size)) {
        vgderr("Failed to set size! " << size);
        return;
    }
    
    if (FT_Load_Char(face, codePoint, FT_LOAD_RENDER)) {
        vgderr("Failed to load glyph! " << codePoint);
        return;
    }


    uint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {
            texture,
            {(int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            (uint)face->glyph->advance.x
    };
    _charMap[_size].insert_or_assign(codePoint, character);
}
