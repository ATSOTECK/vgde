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
