#ifndef __VGDE_SPRITE_H__
#define __VGDE_SPRITE_H__

#include "gl.h"
#include "shader.h"
#include "texture.h"
#include "vec.h"

#include <string>
#include <vector>

class Sprite {
public:
	explicit Sprite(const std::string &spr);
	explicit Sprite(Texture *texture);

	void draw();

	int width() const;
	int height() const;
	vec2i position() const;
	void setPosition(const vec2i &pos);

	vec2i size() const;
	void setSize(const vec2i &size);
	float scale() const;
	void setScale(float scale);

private:
    void init();
    void setVerts();

    Texture *_texture;
	int _width;
	int _height;
	vec2i _position;
	float _scale;
	Shader *_shader;
	std::vector<float> _verts;
};

#endif
