#include "Shader.h"

void ShaderManager::addCustomShader(const std::string& name, const std::string& vertexSourcePath, const std::string& fragmentSourcePath) {

	if (shaderExists(name)) {
		shadersInfo[name] = ShaderInfo{vertexSourcePath, fragmentSourcePath};
	}
}

bool ShaderManager::shaderExists(const std::string& name) const {
	return shadersInfo.find(name) == shadersInfo.end();
}

ShaderInfo& ShaderManager::getShaderInfo(const std::string& name) {
	return shadersInfo[name];
}