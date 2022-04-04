#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include "FourierSeries.h"
#include "BezierCubic.h"
#include "ParametricGeometry.h"
#include "Object3D.h"
#include "Entity.h"
#include "WorldTime.h"
#include "LSystemParams.h"

class PlaneSurface {
private:
	int _uSteps, _vSteps;
public:
	glm::vec3 operator()(float u, float v) const;
};

class Leaf {
	static constexpr float maxAge = 1.0f;
	static constexpr float growthRate = 0.3f;
private:
	float stemGirth;
	float leafHeight;
	float leafAge;
	glm::mat4 basePose;
	glm::mat4 pose;
	glm::vec3 direction;
public:
	Leaf(const glm::mat4& leafPose, const glm::vec3& growthDirection);
	//ParametricGeometry<LeafProfileStart> generateGeometry() const;
	//void act() override;
	void updateStemGirth(float newGirth);
	void updateLeafHeight(float newHeight);
	float grow(const WorldTime& worldTime);
	void act(const WorldTime& worldTime);
	glm::mat4 nextPose();
	glm::mat4 getPose();
};