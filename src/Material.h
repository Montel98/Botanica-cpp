#pragma once

#include "Texture.h"

using TextureId = unsigned int;

struct LightingParams {
	float ambient, diffuse, specular, reflectivity;
};

struct Material {
	LightingParams lighting;
	std::string textureHandle;
};