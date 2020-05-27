#include "sprite.h"

#include "config.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
		vgderr("Failed to load texture")
	}

	stbi_image_free(data);
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
