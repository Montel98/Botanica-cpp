#pragma once

#include <map>
#include <string>
#include <variant>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>
#include <iostream>

//using UniformType = std::variant<glm::ivec1, glm::vec1, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

using UniformType = std::variant<glm::vec1, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

class ShaderInfo {
private:
	std::string loadShaderSource(std::string path) const;
public:
	std::string vertexSource;
	std::string fragmentSource;
	unsigned int programId;

	ShaderInfo(const std::string& vertexPath, const std::string& fragmentPath);
};

struct Shader {
	std::string name;
	std::string vertexSourcePath;
	std::string fragmentSourcePath;
	std::map<std::string, UniformType> uniforms;
};

class ShaderManager {
private:
	std::map<std::string, ShaderInfo> shadersInfo;
public:
	void addCustomShader(const Shader& shader);
	bool shaderExists(const std::string& name) const;
	ShaderInfo& getShaderInfo(const std::string& name);
};