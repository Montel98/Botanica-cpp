#pragma once

#include "FourierSeries.h"
#include <glm/glm.hpp>

class LeafSurface {
private:
	//float foldMag, foldFreq, vMax;
	FourierSeries profileXY;
public:
	LeafSurface(const FourierSeries& fourier/*, int foldFactor, int foldFrequency, int vMax*/);
	glm::vec2 operator()(float u, float v) const;
	//virtual float profileXZ(float x) const = 0;
};

template<typename F>
class PlaneSurface {
private:
	F elevationFunc; // Z value with respect to X
	int _uSteps, _vSteps;
public:
	glm::vec3 operator()(float u, float v) const;
};

template<typename F>
glm::vec3 PlaneSurface<F>::operator()(float u, float v) const {
	return glm::vec3(u, v, elevationFunc(u));
}

class LeafProfileStart {
public:
	float operator()(float x) const;
};

class LeafProfileEnd {
public:
	float operator()(float x) const;
};