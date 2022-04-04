#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "LeafBuilder.h"

namespace TextureBuilder {

Texture generateLeafTexture(unsigned int width, unsigned int height, const LeafSurface& surfaceFunc);
Texture generateStemTexture(unsigned int width, unsigned int height);
float step(float threshold, float x);
float smoothStep(float start, float end, float x);
float clap(float start, float end, float x);
}