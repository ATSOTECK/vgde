#include "sprite.h"

#include "config.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace {
static const std::string defaultFragmentShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"\n"
"void main() {\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"	TexCoord = aTexCoord;\n"
"}";

static const std::string defaultVertexShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"\n"
"uniform sampler2D ourTexture;\n"
"\n"
"void main() {\n"
"	FragColor = texture(ourTexture, TexCoord);\n"
"}";
}

Sprite::Sprite(const std::string &spr) {
	int nrChannels;

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *data = stbi_load(spr.c_str(), &_width, &_height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		vgderr("Failed to load texture");
	}

	stbi_image_free(data);

	_shader = new Shader(defaultVertexShader, defaultFragmentShader, false);
}

void Sprite::draw() {
	//
}

int Sprite::width() const {
	return _width;
}

int Sprite::height() const {
	return _height;
}

vec2i Sprite::position() const {
	return _positon;
}

void Sprite::setPosition(const vec2i &pos) {
	_positon = pos;
}
