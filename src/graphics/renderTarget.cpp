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

#include "renderTarget.h"

#include "draw.h"
#include "gl.h"
#include "util/resourceManager.h"

#include <iostream>

namespace {
const std::string defaultVertexShader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 vert;\n"
        "layout (location = 1) in vec2 vertTexCoord;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 transform;\n"
        "\n"
        "out vec2 fragTexCoord;\n"
        "\n"
        "void main() {\n"
        "   fragTexCoord = vertTexCoord;\n"
        "	gl_Position = projection * transform * vec4(vert, 0, 1.0);\n"
        "}";

const std::string defaultFragmentShader =
        "#version 330 core\n"
        "uniform sampler2D tex;\n"
        "in vec2 fragTexCoord;\n"
        "out vec4 finalColor;\n"
        "\n"
        "void main() {\n"
        "	finalColor = texture(tex, fragTexCoord);\n"
        "}";

const GLuint elements[] ={
        0, 1, 2,
        2, 3, 0
};
}

RenderTarget::RenderTarget(const vec2f &size) :
    Transform(),
    _frameBuffer(0),
    _textureID(0),
    _vbo(0),
    _ebo(0)
{
    setSize(size);
    
    //glGenFramebuffers(1, &_frameBuffer);
    glCreateFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    //glGenTextures(1, &_textureID);
    glCreateTextures(GL_TEXTURE_2D, 1, &_textureID);
    bindTexture();
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, null);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    unbindTexture();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureID, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        vgderr("Framebuffer not complete!");
    }

    unbind();
    
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    
    _shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
    
    _shader->setMat4("transform", _transform, true);
    
    glm::mat4 projection = drawGetProjection();
    _shader->setMat4("projection", projection, true);
    
    ResourceManager::instance()->addShader(_shader);
    
    setPosition({100, 100});
}

RenderTarget::~RenderTarget() {
    glDeleteTextures(1, &_textureID);
    glDeleteFramebuffers(1, &_frameBuffer);
    _shader->stop();
    ResourceManager::instance()->removeShader(_shader);
}

void RenderTarget::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    //TODO("Skyler", "Position?");
    //glViewport(0, 0, _size.x, _size.y);
    //glViewport(0, 0, 1024, 600);
}

void RenderTarget::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, 1024, 600);
}

void RenderTarget::clear(const Color &color) {
    Color c = drawGetClearColor();
    glClearColor(color.glR, color.glG, color.glB, color.glA);
    glClear(GL_COLOR_BUFFER_BIT);
    drawSetClearColor(c);
}

void RenderTarget::draw() {
    if (_textureID == 0 || _size.x == 0 || _size.y == 0) {
        return;
    }
    
    if (_updateTransform) {
        _shader->setMat4("transform", _transform, true);
        _updateTransform = false;
    }
    
    float x = _position.x;
    float y = _position.y;
    float w = _size.x;
    float h = _size.y;
    
    float verts[] = {
            //  Position   Texcoords
            x,      y + h, 0.0f, 0.0f, // Top-left
            x + w,  y + h, 1.0f, 0.0f, // Top-right
            x + w,  y,     1.0f, 1.0f, // Bottom-right
            x,      y,     0.0f, 1.0f  // Bottom-left
    };
    
    _shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &verts[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), null);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(sizeof(float) * 2));
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), elements, GL_STATIC_DRAW);
    
    bindTexture();
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);
    
    unbindTexture();
    _shader->stop();
    
    glBindVertexArray(0);
}

void RenderTarget::bindTexture() const {
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void RenderTarget::unbindTexture() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
