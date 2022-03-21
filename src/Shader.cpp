#include "Shader.h"

void ShaderManager::addCustomShader(const Shader& shader) {

	if (!shaderExists(shader.name)) {
		shadersInfo.emplace(std::make_pair(shader.name, ShaderInfo(shader.vertexSourcePath, shader.fragmentSourcePath)));
	}
}

bool ShaderManager::shaderExists(const std::string& name) const {
	return shadersInfo.find(name) != shadersInfo.end();
}

ShaderInfo& ShaderManager::getShaderInfo(const std::string& name) {
	return shadersInfo.at(name);
}

ShaderInfo::ShaderInfo(const std::string& vertexPath, const std::string& fragmentPath) :
vertexSource(loadShaderSource(vertexPath)), fragmentSource(loadShaderSource(fragmentPath)), programId(-1) {}

// Loads shader from path specified
std::string ShaderInfo::loadShaderSource(std::string path) const {
	std::ostringstream ss;
	std::ifstream shaderFile(path);

	if (!shaderFile.is_open()) {
		std::cout << "Error opening shader at " << path << "\n"; 
	}
	else {
		ss << shaderFile.rdbuf();
		std::cout << ss.str() << "\n";
	}

	return ss.str();
}

Shader::Shader(std::string shaderName, std::string vertexPath, std::string fragmentPath) 
: name(std::move(shaderName)), vertexSourcePath(std::move(vertexPath)), fragmentSourcePath(std::move(fragmentPath)) {}