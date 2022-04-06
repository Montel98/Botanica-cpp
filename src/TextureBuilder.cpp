#include "TextureBuilder.h"
#include <cmath>

namespace TextureBuilder {

Texture generateLeafTexture(unsigned int width, unsigned int height, const LeafSurface& surfaceFunc) {

	std::vector<unsigned char> buffer(width * height * 4);
	int index = 0;
	float threshold = 1.0f;

	for (int y = 0; y < height; y++) {

		float yNorm = ((2.0f * y) / (height - 1)) - 1.0f;

		for (int x = width - 1; x >= 0; x--) {

			float xNorm = ((2.0f * x) / (width - 1)) - 1.0f;

			float angle = M_PI + std::atan2(yNorm, xNorm);
			float distance = std::sqrt(xNorm * xNorm + yNorm * yNorm);

			glm::vec2 point = surfaceFunc(angle, distance);
			glm::vec2 target = surfaceFunc(angle, threshold);

			unsigned char alpha = 255 * (1 - step(glm::length(target), distance));
			//unsigned char colour = 255 * distance;

			buffer[index] = 255 * 0.6f;
			buffer[index + 1] = 255 * 0.6f;
			buffer[index + 2] = 255 * 0.4f;
			buffer[index + 3] = alpha;
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
		"uLeafTexture"
	};

	return texture;
}

Texture generateStemTexture(unsigned int width, unsigned int height) {

	std::vector<unsigned char> buffer(width * height * 4);
	int index = 0;

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			buffer[index] = 54;
			buffer[index + 1] = 51;//255;
			buffer[index + 2] = 50;//0;
			buffer[index + 3] = 255;//255;
			//54, 51, 50
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
		"uStemTexture"
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