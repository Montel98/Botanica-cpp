#include "TextureManager.h"

TextureManager::TextureManager() {}

void TextureManager::addTexture(Texture newTexture, const std::string& textureName) {
	textures.emplace(std::make_pair(textureName, std::move(newTexture)));
}

void TextureManager::removeTexture(const std::string& textureName) {
	textures.erase(textureName);
}

Texture& TextureManager::getTextureById(const std::string& textureName) {
	return textures.at(textureName);
}