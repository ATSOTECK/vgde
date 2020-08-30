#ifndef __VGDE_SPRITE_H__
#define __VGDE_SPRITE_H__

#include "gl.h"
#include "shader.h"
#include "graphics/texture.h"
#include "vec.h"

#include <string>
#include <vector>

class Sprite {
public:
	explicit Sprite(const std::string &spr);
	explicit Sprite(Texture *texture);
	~Sprite();

	void draw();

	float width() const;
	float height() const;
	vec2f position() const;
	void setPosition(const vec2f &pos);

	vec2f size() const;
	void setSize(const vec2f &size);
	vec2f scale() const;
	void setScale(const vec2f &scale);

	void setRotation(float angle);

	void setOrigin(const vec2f &origin);

private:
    void init();
    void setVerts();

    Texture *_texture;
	vec2f _size;
	vec2f _position;
	vec2f _origin;
	vec2f _scale;
	Shader *_shader;
	std::vector<float> _verts;
	uint _vbo;
	uint _ebo;

	glm::mat4 _transform;
};

#endif
