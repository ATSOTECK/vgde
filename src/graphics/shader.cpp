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

#include "shader.h"

#include "config.h"

#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vert, const std::string &frag, bool path) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertFile;
	std::ifstream fragFile;

	if (path) {
		vertFile.open(vert);
		fragFile.open(frag);
		
		if (!vertFile.is_open()) {
			vgderr("Could not open vertex shader.");
			return;
		}

		if (!fragFile.is_open()) {
			vgderr("Could not open fragment shader.");
			return;
		}

		std::stringstream vertStream, fragStream;
		
		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();
		
		vertFile.close();
		fragFile.close();
		
		vertexCode = vertStream.str();
		fragmentCode = fragStream.str();
	} else {
		vertexCode = vert;
		fragmentCode = frag;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	GLuint vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);
	glValidateProgram(_id);
	checkCompileErrors(_id, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(_id);
}

void Shader::stop() {
    glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool val) const {
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)val);
}

void Shader::setInt(const std::string &name, int val) const {
	glUniform1i(glGetUniformLocation(_id, name.c_str()), val);
}

void Shader::setFloat(const std::string &name, float val) const {
	glUniform1f(glGetUniformLocation(_id, name.c_str()), val);
}

void Shader::setMat4(const std::string &name, glm::mat4 &val, bool useShader) {
	if (useShader) {
		use();
	}

	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, false, glm::value_ptr(val));
}

void Shader::setVec3f(const std::string &name, const vec3f &vec, bool useShader) {
    if (useShader) {
        use();
    }

    glUniform3f(glGetUniformLocation(_id, name.c_str()), vec.x, vec.y, vec.z);
}

uint Shader::getUniformLocation(const std::string &name) {
    return glGetUniformLocation(_id, name.c_str());
}

void Shader::checkCompileErrors(unsigned int shader, const std::string &type) {
	int success;
	char log[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, log);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << log 
				<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, log);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << log 
				<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
