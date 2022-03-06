#pragma once

#include <map>
#include <string>
#include <variant>
#include <glm/glm.hpp>

//using UniformType = std::variant<glm::ivec1, glm::vec1, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

using UniformType = std::variant<glm::vec1, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

struct ShaderInfo {
	std::string vertexSourcePath;
	std::string fragmentSourcePath;
	unsigned int programId;
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
	void addCustomShader(const std::string& name, const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	bool shaderExists(const std::string& name) const;
	ShaderInfo& getShaderInfo(const std::string& name);
};