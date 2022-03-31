#include "Leaf.h"
#include "FourierSeries.h"
#include "ParametricGeometry.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define PI 3.14159265

LeafSurface::LeafSurface(const FourierSeries &fourier, int foldFactor, int foldFrequency, int vMax) :
profileXY(std::move(fourier)), foldMag(foldFactor), foldFreq(foldFrequency), vMax(vMax) {};

glm::vec3 LeafSurface::operator()(float u, float v) const {
	float r = v * profileXY.sum(u);
	float x = (0.14 * r * cos(u)) - (vMax * profileXY.sum(PI) * cos(PI)) + 0.005;
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

/*Leaf::Leaf() : geometry(generateGeometry()) {
}*/

Leaf::Leaf(const glm::mat4& leafPose) : leafAge(0.0f), stemGirth(0.0f), 
pose(leafPose), basePose(leafPose) {};

void Leaf::updateStemGirth(float newGirth) {
	stemGirth = newGirth;
}

float Leaf::grow(const WorldTime& worldTime) {
	float newAge = leafAge + Leaf::growthRate * worldTime.dt();

	if (newAge > Leaf::maxAge) {
		newAge = Leaf::maxAge;
	}
	return newAge;
}

void Leaf::act(const WorldTime& worldTime) {
	leafAge = grow(worldTime);
	pose = basePose * glm::translate(glm::vec3(stemGirth, 0, 0));
}

glm::mat4 Leaf::getPose() {
	return pose;
}

ParametricGeometry<LeafProfileStart> Leaf::generateGeometry() const {

	float magnitudeA = 1.0;
	float magnitudeB = 2.0;

	// Construct leaf shape with fourier terms
	std::vector<FourierTerm> terms;
	terms.push_back(FourierTerm(0.0, 0.5, 0.5, 2.0));
	terms.push_back(FourierTerm(0.0, 0.3, 0.5 * magnitudeA, 2.0));
	terms.push_back(FourierTerm(0.0, 0.2, 0.5 * magnitudeB, 2.0));

	FourierSeries fourier(0.0, terms);

	// Define leaf profile functions (keyframes)
	LeafProfileStart profileStart(fourier, 0.1, 10.0, 0.2);
	LeafProfileEnd profileEnd(fourier, 0.02, 20.0, 0.2);

	GeometryConstraints constraints = {0.0, 1.0, 0.0, 2.0 * PI, 64, 4};

	// Generate leaf geometry
	ParametricGeometry<LeafProfileStart> leafGeometryStart(profileStart, constraints);
	leafGeometryStart.useNormals(false);
	leafGeometryStart.useSTs();
	leafGeometryStart.generateGeometry();

	return leafGeometryStart;
}

glm::vec3 PlaneSurface::operator()(float u, float v) const {
	return glm::vec3(u, v, 0.0);
}