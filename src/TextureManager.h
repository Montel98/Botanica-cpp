#pragma once

#include "Texture.h"
#include <map>
#include <string>

class TextureManager {
private:
	std::map<std::string, Texture> textures;
public:
	TextureManager();
	void addTexture(Texture texture, const std::string& textureName);
	void removeTexture(const std::string& textureName);
	Texture& getTextureById(const std::string& textureName);
};