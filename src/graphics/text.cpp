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

#include "text.h"

#include "draw.h"
#include "util/resourceManager.h"

#include <utility>

//TODO(Skyler): Find a better way to do this.
namespace {
const std::string textVertShader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 vertex;\n"
        "layout (location = 1) in vec2 vertTexCoord;\n"
        "out vec2 texCoords;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main() {\n"
        "   gl_Position = projection * vec4(vertex.xy, 0, 1.0);\n"
        "   texCoords = vertTexCoord;\n"
        "}";

const std::string textFragShader =
        "#version 330 core\n"
        "in vec2 texCoords;\n"
        "out vec4 color;\n"
        "uniform sampler2D text;\n"
        "uniform vec3 textColor;\n"
        "\n"
        "void main() {\n"
        "   vec4 sampled = vec4(1, 1, 1, texture(text, texCoords).r);\n"
        "   color = vec4(textColor, 1) * sampled;\n"
        "}";
}

Text::Text(const String &str):
    Transform()
{
    _font = ResourceManager::instance()->defaultFont();
    _shader = new Shader(textVertShader, textFragShader, false);
    glm::mat4 projection = drawGetProjection();
    _shader->setMat4("projection", projection, true);
    _scale = 1.f;
    setText(str);
    //calculateVerts();
}

Text::Text(const String &str, Font *font):
    Transform()
{
    _position = {200, 100};
    _font = font;
    _shader = new Shader(textVertShader, textFragShader, false);
    glm::mat4 projection = drawGetProjection();
    _shader->setMat4("projection", projection, true);
    _scale = 1.f;
    setText(str);
    _color = Color::White;
    //calculateVerts();
}

Text::~Text() {
    for (float *v : _verts) {
        delete[] v;
    }
    
    _verts.clear();
}

void Text::setText(const String &txt) {
    if (txt == _text) {
        return;
    }
    
    _text = txt;
    
    if (_verts.size() < _text.length()) {
        _verts.reserve(_text.length());
        _chars.reserve(_text.length());
        
        for (size_t i = _verts.size(); i < _text.length(); ++i) {
            var vert = new float[16];
            _verts.push_back(vert);
        }
    }
    
    calculateVerts();
}

String Text::text() const {
    return _text;
}

void Text::setFont(Font *font) {
    _font = font;
}

Font *Text::font() const {
    return _font;
}

int Text::length() const {
    return _text.length();
}

void Text::draw() {
    uint vbo = _font->vbo();
    uint ibo = _font->ibo();
    
    _shader->use();
    _shader->setVec3f("textColor", _color.vec3gl());
    
    for (size_t i = 0; i < _chars.size(); ++i) {
        Character ch = _chars[i];
    
        uint indices[] = {
                0, 1, 2,
                2, 3, 0
        };
    
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
    
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), _verts[i], GL_STATIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);
    
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(sizeof(float) * 2));
    
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
    }
    
    _shader->stop();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::calculateVerts() {
    float x = _position.x;
    float y = _position.y;
    float ox = x;
    _size.x = 0;
    int size = _font->size();
    _chars.clear();
    
    for (int i = 0; i < _text.length(); ++i) {
        uint32 cp = _text[i];
    
        if (cp == '\n') {
            y += (float)size;
            x = ox;
            continue;
        }
    
        if (cp == '\t') {
            x += ((_font->getChar(size, ' ').advance >> 6u) * _scale) * 4;
            continue;
        }
    
        /*
        if (cp == '[' && _text[i - 1] != '\\') {
            String str;
            while (cp != ']') {
                cp = _text[++i];
                if (cp != ']') {
                    str += cp;
                }
            }
        
            if (str.empty()) {
                _shader->setVec3f("textColor", _color.vec3gl());
            } else if (str == "black") {
                _shader->setVec3f("textColor", Color::Black.vec3gl());
            } else if (str == "white") {
                _shader->setVec3f("textColor", Color::White.vec3gl());
            } else if (str == "red") {
                _shader->setVec3f("textColor", Color::Red.vec3gl());
            } else if (str == "green") {
                _shader->setVec3f("textColor", Color::Green.vec3gl());
            } else if (str == "blue") {
                _shader->setVec3f("textColor", Color::Blue.vec3gl());
            } else if (str == "lightBlue") {
                _shader->setVec3f("textColor", Color::LightBlue.vec3gl());
            } else if (str == "yellow") {
                _shader->setVec3f("textColor", Color::Yellow.vec3gl());
            } else if (str == "magenta") {
                _shader->setVec3f("textColor", Color::Magenta.vec3gl());
            } else if (str == "cyan") {
                _shader->setVec3f("textColor", Color::Cyan.vec3gl());
            } else if (str == "turquoise") {
                _shader->setVec3f("textColor", Color::Turquoise.vec3gl());
            }
        
            continue;
        }
         */
    
        Character ch = _font->getChar(size, cp);
        if (ch.textureID == 0) {
            _font->getGlyph(cp, size);
        
            ch = _font->getChar(size, cp);
        }
        
        _chars.push_back(ch);
    
        float xpos = x + ch.bearing.x * _scale;
        float ypos = y - (ch.bearing.y * _scale) + (float)size;
        float w = ch.size.x * _scale;
        float h = ch.size.y * _scale;
        
        _verts[i][0]  = xpos;     _verts[i][1]  = ypos + h; _verts[i][2]  = 0.0f; _verts[i][3] = 1.0f;
        _verts[i][4]  = xpos + w; _verts[i][5]  = ypos + h; _verts[i][6]  = 1.0f; _verts[i][7] = 1.0f;
        _verts[i][8]  = xpos + w; _verts[i][9]  = ypos;     _verts[i][10] = 1.0f; _verts[i][11] = 0.0f;
        _verts[i][12] = xpos;     _verts[i][13] = ypos;     _verts[i][14] = 0.0f; _verts[i][15] = 0.0f;
    
        x += (ch.advance >> 6u) * _scale;
    
        if (x > _size.x) {
            _size.x = x;
        }
    }
}
