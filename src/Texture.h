#pragma once

enum TextureInterpolation { Linear, NearestNeighbour };
enum TextureType { Unsigned, Float };

struct Texture {
	TextureInterpolation interpolation;
	TextureType type;
	unsigned int width;
	unsigned int height;
	unsigned int textureId;
};