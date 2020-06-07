#ifndef __VGDE_SHADER_H__
#define __VGDE_SHADER_H__

//Based on https://learnopengl.com/Getting-started/Shaders

#include "gl.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
	Shader(const std::string &vert, const std::string &frag, bool path = true);

	void use();

	void setBool(const std::string &name, bool val) const;
	void setInt(const std::string &name, int val) const;
	void setFloat(const std::string &name, float val) const;
	void setMat4(const std::string &name, glm::mat4 &val, bool useShader = false);

private:
	unsigned int _id;
	void checkCompileErrors(unsigned int shader, const std::string &type);
};

#endif
