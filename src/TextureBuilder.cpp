#include "TextureBuilder.h"
#include <cmath>

namespace TextureBuilder {

Texture generateLeafTexture(unsigned int width, unsigned int height, const LeafSurface& surfaceFunc) {

	std::vector<unsigned int> buffer(width * height * 4);
	int index = 0;

	float threshold = 0.95f;

	for (int x = 0; x < width; x++) {

		float xNorm = ((2.0f * x) / (width - 1)) - 1.0f;

		for (int y = 0; y < height; y++) {

			float yNorm = ((2.0f * y) / (height - 1)) - 1.0f;

			float angle = M_PI + atan2(yNorm, xNorm);
			float distance = sqrt(xNorm * xNorm + yNorm * yNorm);

			glm::vec2 point = surfaceFunc(angle, distance);
			glm::vec2 target = surfaceFunc(angle, threshold);

			float colour = 1.0f - step(glm::length(target), glm::length(point));

			buffer[index] = colour;
			buffer[index + 1] = colour;
			buffer[index + 2] = colour;
			buffer[index + 3] = colour;
			index += 4;
		}
	}

	Texture texture{
		TextureInterpolation::Linear,
		TextureType::Unsigned, 
		width, 
		height, 
		-1, 
		std::move(buffer),
		"uSampler"
	};

	return texture;
}

Texture generateStemTexture(unsigned int width, unsigned int height) {

	std::vector<unsigned int> buffer(width * height * 4);
	int index = 0;

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			buffer[index] = 255;
			buffer[index + 1] = 255;
			buffer[index + 2] = 255;
			buffer[index + 3] = 255;
			index += 4;
		}
	}

	Texture texture{
		TextureInterpolation::Linear,
		TextureType::Unsigned,
		width,
		height,
		-1,
		std::move(buffer),
		"Stem"
	};

	return texture;
}

float step(float threshold, float x) {
	return x >= threshold ? 1 : 0;
}

float smoothStep(float start, float end, float x) {
	if (x >= start) {
		if (x <= end) {
			return (x - start) / (end - start);
		}
		return 1;
	}
	return 0;
}

float clamp(float start, float end, float x) {
	if (x >= start) {
		if (x <= end) {
			return x;
		}
		return end;
	}
	return start;
}
}