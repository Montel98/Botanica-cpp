#pragma once

#include "Texture.h"

struct LightingParams {
	float ambient, diffuse, specular, reflectivity;
};

struct Material {
	LightingParams lighting;
	Texture textureMap;
};