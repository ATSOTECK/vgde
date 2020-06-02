#ifndef __VGDE_SPRITE_H__
#define __VGDE_SPRITE_H__

//Based on https://learnopengl.com/Getting-started/Textures

#include "gl.h"
#include "shader.h"
#include "vec.h"

#include <string>

class Sprite {
public:
	Sprite(const std::string &spr);

	void draw();

	int width() const;
	int height() const;
	vec2i position() const;
	void setPosition(const vec2i &pos);

private:
	int _width;
	int _height;
	GLuint _texture;
	vec2i _positon;
	Shader *_shader;
};

#endif
