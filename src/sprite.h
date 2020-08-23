#ifndef __VGDE_SPRITE_H__
#define __VGDE_SPRITE_H__

//Based on https://learnopengl.com/Getting-started/Textures

#include "gl.h"
#include "shader.h"
#include "texture.h"
#include "vec.h"

#include <string>

class Sprite {
public:
	explicit Sprite(const std::string &spr);
	explicit Sprite(Texture *texture);

	void draw();

	int width() const;
	int height() const;
	vec2i position() const;
	void setPosition(const vec2i &pos);

private:
    void init();

    Texture *_texture;
	int _width;
	int _height;
	vec2i _position;
	Shader *_shader;
};

#endif
