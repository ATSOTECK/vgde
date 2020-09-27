#include "font.h"

#include "color.h"
#include "gl.h"

#include <iostream>

Font::Font(const std::string &fnt) {
    loadFont(fnt);
}

void Font::loadFont(const std::string &filename) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        vgderr("Could not init freetype!");
    }
    _lib = ft;

    FT_Face face;
    if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
        vgderr("Failed to load font!");
    }
    _face = face;

    FT_Set_Pixel_Sizes(face, 0, DEFAULT_FONT_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (uint c = 0; c < 128; ++c) {
        getGlyph(c, 24);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

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
}

void Font::draw(const std::string &txt, float x, float y, float scale, Shader *shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);
    float ox = x;

    for (int i = 0; i < txt.size(); ++i) {
        if (txt[i] == '\n') {
            y += 24;
            x = ox;
            continue;
        }

        if (txt[i] == '\t') {
            x += ((_chars[' '].advance >> 6) * scale) * 4;
            continue;
        }

        if (txt[i] == '[' && txt[i - 1] != '\\') {
            std::string str;
            while (txt[++i] != ']') {
                str += txt[i];
            }
            ++i;

            if (str == "black") {
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
        }

        Character ch = _chars[txt[i]];
        if (ch.textureID == 0) {
            getGlyph(txt[i], 24);
            ch = _chars[txt[i]];
        }

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.bearing.y * scale) + 24; //TODO(Skyler): '24' is the font size. Move this over to a font file.
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
        };*/

        uint indicies[] = {
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

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
        x += (ch.advance >> 6) * scale;
    }
}

void Font::getGlyph(uint codePoint, int size, bool bold, float outlineThickness) {
    FT_Face face = (FT_Face)_face;
    if (FT_Load_Char(face, codePoint, FT_LOAD_RENDER)) {
        vgderr("Failed to load glyph!");
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
    _chars.insert(std::pair<uint, Character>(codePoint, character));
}
