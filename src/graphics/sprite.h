#ifndef __VGDE_SPRITE_H__
#define __VGDE_SPRITE_H__

#include "gl.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "vec.h"

#include <string>
#include <vector>

class Sprite : public Transform {
public:
	explicit Sprite(const std::string &spr);
	explicit Sprite(Texture *texture);
	~Sprite();

	void draw();

private:
    void init();
    void setVerts();

    Texture *_texture;
	Shader *_shader;
	std::vector<float> _verts;
	uint _vbo;
	uint _ebo;
};

#endif
