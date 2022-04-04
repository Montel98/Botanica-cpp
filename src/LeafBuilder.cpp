#include "LeafBuilder.h"
#include <cmath>

LeafSurface::LeafSurface(const FourierSeries &fourier, int foldFactor, int foldFrequency, int vMax) :
profileXY(std::move(fourier)), foldMag(foldFactor), foldFreq(foldFrequency), vMax(vMax) {};

glm::vec3 LeafSurface::operator()(float u, float v) const {
	float r = v * profileXY.sum(u);
	float x = (0.14 * r * cos(u)) - (vMax * profileXY.sum(M_PI) * cos(M_PI)) + 0.005;
	float y = 0.12 * r * sin(u);
	float z = 0.14 * abs(foldMag * sin(foldFreq * r * sin(u))) - profileXZ(r * cos(u));

	return glm::vec3(x, y, z);
}

LeafProfileStart::LeafProfileStart(const FourierSeries &fourier, int foldFactor, int foldFrequency, int vMax)
: LeafSurface(fourier, foldFactor, foldFrequency, vMax) {};

float LeafProfileStart::profileXZ(float x) const {
	return -0.4*x;
}

LeafProfileEnd::LeafProfileEnd(const FourierSeries &fourier, int foldFactor, int foldFrequency, int vMax)
: LeafSurface(fourier, foldFactor, foldFrequency, vMax) {};

float LeafProfileEnd::profileXZ(float x) const {
	return pow(0.4*x, 2.0);
}