#pragma once

#include <vector>
#include <string>

enum class TextureInterpolation { Linear, NearestNeighbour };
enum class TextureType { Unsigned, Float };

struct Texture {
	TextureInterpolation interpolation;
	TextureType type;
	unsigned int width;
	unsigned int height;
	int textureId = -1;
	std::vector<unsigned int> bufferData;
	std::string name;
};