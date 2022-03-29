#pragma once

#include <glm/glm.hpp>
#include <math.h>
#include "FourierSeries.h"
#include "BezierCubic.h"
#include "ParametricGeometry.h"
#include "Object3D.h"
#include "Entity.h"
#include "WorldTime.h"

class PlaneSurface {
private:
	int _uSteps, _vSteps;
public:
	glm::vec3 operator()(float u, float v) const;
};

class LeafSurface {
private:
	float foldMag, foldFreq, vMax;
	FourierSeries profileXY;
public:
	LeafSurface(const FourierSeries& fourier, int foldFactor, int foldFrequency, int vMax);
	glm::vec3 operator()(float u, float v) const;
	virtual float profileXZ(float x) const = 0;
};


class LeafProfileStart : public LeafSurface {
public:
	LeafProfileStart(const FourierSeries& fourier, int foldFactor, int foldFrequency, int vMax);
	float profileXZ(float x) const;
};

class LeafProfileEnd : public LeafSurface {
public:
	LeafProfileEnd(const FourierSeries& fourier, int foldFactor, int foldFrequency, int vMax);
	float profileXZ(float x) const;
};

class Leaves : public Entity {
public:
	Leaves();
	Mesh generatePlane() const;
	void act(const WorldTime& worldTime) override;
	void addLeaf(const glm::mat4& pose);
};

class Leaf {
	//Object3D worldObject;
public:
	Leaf();
	ParametricGeometry<LeafProfileStart> generateGeometry() const;
	//void act() override;
	//ParametricGeometry<LeafProfileStart> geometry;
};