#include "LeafBuilder.h"
#include <cmath>

LeafSurface::LeafSurface(const FourierSeries &fourier/*, int foldFactor, int foldFrequency, int vMax*/) :
profileXY(std::move(fourier))/*, foldMag(foldFactor), foldFreq(foldFrequency), vMax(vMax)*/ {};

glm::vec2 LeafSurface::operator()(float u, float v) const {
	float r = v * profileXY.sum(u);
	float x = r * std::cos(u);
	float y = r * std::sin(u);

	return glm::vec2(x, y);
}

float LeafProfileStart::operator()(float x) const {
	return -x;
}

float LeafProfileEnd::operator()(float x) const {
	return 20.0f * x * x;
}