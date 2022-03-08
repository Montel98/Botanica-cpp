#pragma once

#include <vector>
#include <string>

enum TextureInterpolation { Linear, NearestNeighbour };
enum TextureType { Unsigned, Float };

struct Texture {
	TextureInterpolation interpolation;
	TextureType type;
	unsigned int width;
	unsigned int height;
	int textureId = -1;
	std::vector<unsigned int> bufferData;
	std::string name;
};