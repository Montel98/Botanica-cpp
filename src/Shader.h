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

class Shader {
public:
	std::string name;
	std::string vertexSourcePath;
	std::string fragmentSourcePath;
	std::map<std::string, UniformType> uniforms;

	Shader(std::string shaderName, std::string vertexPath, std::string fragmentPath);

	template<typename T>
	void addUniform(const std::string& name, const T& uniform);

	template<typename T>
	T getUniform(const std::string& name);

	template<typename T>
	void setUniform(const std::string& name, const T& newValue);
};

class ShaderManager {
private:
	std::map<std::string, ShaderInfo> shadersInfo;
public:
	void addCustomShader(const Shader& shader);
	bool shaderExists(const std::string& name) const;
	ShaderInfo& getShaderInfo(const std::string& name);
};

template<typename T>
void Shader::addUniform(const std::string& name, const T& uniform) {
	uniforms.insert(std::make_pair(name, UniformType{uniform}));
}

template<typename T>
T Shader::getUniform(const std::string& name) {
	return std::get<T>(uniforms.at(name));
}

template<typename T>
void Shader::setUniform(const std::string& name, const T& newValue) {
	std::get<T>(uniforms.at(name)) = newValue;
}