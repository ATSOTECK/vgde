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

#ifndef __VGDE_SHADER_H__
#define __VGDE_SHADER_H__

//Based on https://learnopengl.com/Getting-started/Shaders

#include "gl.h"
#include "config.h"
#include "vec.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
	Shader(const std::string &vert, const std::string &frag, bool path = true);

	void use();
	void stop();

	void setBool(const std::string &name, bool val) const;
	void setInt(const std::string &name, int val) const;
	void setFloat(const std::string &name, float val) const;
	void setMat4(const std::string &name, glm::mat4 &val, bool useShader = false);
	void setVec3f(const std::string &name, const vec3f &vec, bool useShader = false);

	uint getUniformLocation(const std::string &name);

private:
	unsigned int _id;
	void checkCompileErrors(unsigned int shader, const std::string &type);
};

#endif
