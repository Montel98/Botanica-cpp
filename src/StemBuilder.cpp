#include "StemBuilder.h"
#include <memory>
#include <math.h>

#define BRANCH_LENGTH 0
#define PI 3.14

namespace StemBuilder {

StemRadius::StemRadius(float rStart, float rEnd, int branchLength, int shift) 
: radiusStart(rStart), radiusEnd(rEnd), noStems(branchLength), _shift(shift) {

	if (radiusEnd == radiusStart) {
		decayRate = 0.0;
	}
	else {
		decayRate = (-1.0 / noStems) * log((0.9 * radiusEnd) / (radiusStart - radiusEnd));
	}
}

float StemRadius::baseRadius(float u) const {
	return radiusEnd + (radiusStart - radiusEnd) * exp(-decayRate * (u + _shift));
}

StemBodyRadius::StemBodyRadius(float rStart, float rEnd, int branchLength, int shift) 
: StemRadius(rStart, rEnd, branchLength, shift) {}

float StemBodyRadius::operator()(float u) const {
	return baseRadius(u);
}

StemBodyRadius* StemBodyRadius::clone() const {
	return new StemBodyRadius(*this);
}

StemTipRadiusStart::StemTipRadiusStart(float rStart, float rEnd, int branchLength, int shift) 
: StemRadius(rStart, rEnd, branchLength, shift) {}

float StemTipRadiusStart::operator()(float u) const {
	return baseRadius(0.0) * (float)(1.0 - pow(u, 2.0));
}

StemTipRadiusStart* StemTipRadiusStart::clone() const {
	return new StemTipRadiusStart(*this);
}

StemTipRadiusEnd::StemTipRadiusEnd(float rStart, float rEnd, int branchLength, int shift) 
: StemRadius(rStart, rEnd, branchLength, shift) {}

float StemTipRadiusEnd::operator()(float u) const {
	return baseRadius(1.0) * (float)(1.0 - pow(u, 2.0)); 
}

StemTipRadiusEnd* StemTipRadiusEnd::clone() const {
	return new StemTipRadiusEnd(*this);
}

StemSurface::StemSurface(std::unique_ptr<StemRadius> radiusFunc, const BezierLinear& path, const Axis& axis)
: _radiusFunc(std::move(radiusFunc)), _path(path), _axis(axis) {}

StemSurface::StemSurface(const StemSurface& other) : _path(other._path), _axis(other._axis) {
	this->_radiusFunc = std::unique_ptr<StemRadius>(other._radiusFunc->clone());
}

glm::vec3 StemSurface::operator()(float u, float v) const {
	glm::vec3 bezierPoint = _path.eval(u);
	glm::vec3 bezierGradient = _path.derivative(u);

	glm::vec3 crossSectionPoint = crossSection(v);

	return bezierPoint + crossSectionPoint;
}

glm::vec3 StemSurface::crossSection(float v) const {
	float radius = _radiusFunc->operator()(v);
	glm::vec3 position = _axis.left * radius * cos(v) + _axis.up * radius * sin(v);

	return position * (0.9f + 0.1f * pow(cos(3.0f * v), 2.0f));
}

StemSurfaceMorphInfo::StemSurfaceMorphInfo(
	std::unique_ptr<StemRadius> rStart, 
	std::unique_ptr<StemRadius> rEnd, 
	BezierLinear pathBegin, 
	BezierLinear pathFinish
	) :
	radiusStart(std::move(rStart)),
	radiusEnd(std::move(rEnd)), 
	pathStart(std::move(pathBegin)), 
	pathEnd(std::move(pathFinish)) {};

StemSurfaceMorphInfo::StemSurfaceMorphInfo(const StemSurfaceMorphInfo& other) : 
pathStart(other.pathStart), pathEnd(other.pathEnd) {

	radiusStart = std::unique_ptr<StemRadius>(other.radiusStart->clone());
	radiusEnd = std::unique_ptr<StemRadius>(other.radiusEnd->clone());
}

ParametricGeometry<StemSurface> generateStemBody(const LSystemParams &lParams) {

	glm::vec3 normDir = glm::normalize(lParams.axis.forward);
	glm::vec3 p0 = lParams.position;
	glm::vec3 p1 = lParams.position + 0.03f * normDir;

	BezierLinear stemBodyPath(p0, p1);
	BezierLinear immatureStemBodyPath(p0, p0);

	StemBodyRadius startBodyRadiusFunc(0.001f, 0.001f, BRANCH_LENGTH, 0);

	StemBodyRadius endBodyRadiusFunc(lParams.radiusStart,
									lParams.radiusEnd,
									BRANCH_LENGTH,
									lParams.count);

	GeometryConstraints stemConstraints = {0.0, 1.0, 0.0, 2.0 * PI, 2, 10};

	StemSurfaceMorphInfo keyFrameInfo(
		std::make_unique<StemBodyRadius>(startBodyRadiusFunc), 
		std::make_unique<StemBodyRadius>(endBodyRadiusFunc), 
		stemBodyPath,
		immatureStemBodyPath
	);

	return StemBuilder::generateStemGeometry(std::move(keyFrameInfo), lParams.axis, stemConstraints);
}

ParametricGeometry<StemSurface> generateStemTip(const LSystemParams &lParams) {

	// Define the stem tip path direction in 3D space
	glm::vec3 normDir = glm::normalize(lParams.axis.forward);
	glm::vec3 p0 = lParams.position;
	glm::vec3 p1 = lParams.position + 0.03f * normDir;
	glm::vec3 p2 = lParams.position + 0.042f * normDir;
	glm::vec3 p3 = lParams.position + 0.012f * normDir;

	BezierLinear stemTipPath(p1, p2);
	BezierLinear immatureStemTipPath(p0, p3);

	// Define the parametric functions for radius over length of tip
	StemTipRadiusStart startTipRadiusFunc(0.001f, 0.001f, BRANCH_LENGTH, 0);

	StemTipRadiusEnd endTipRadiusFunc(lParams.radiusStart,
									lParams.radiusEnd,
									BRANCH_LENGTH,
									lParams.count);

	GeometryConstraints stemConstraints = {0.0, 1.0, 0.0, 2.0 * PI, 3, 10};

	StemSurfaceMorphInfo keyFrameInfo(
		std::make_unique<StemTipRadiusStart>(startTipRadiusFunc), 
		std::make_unique<StemTipRadiusEnd>(endTipRadiusFunc), 
		stemTipPath,
		immatureStemTipPath
	);

	return StemBuilder::generateStemGeometry(std::move(keyFrameInfo), lParams.axis, stemConstraints);
}

ParametricGeometry<StemSurface> generateStemGeometry(StemSurfaceMorphInfo keyFrameInfo,
																const Axis& axis,
																const GeometryConstraints& constraints) {
	// Main geometry
	StemSurface endSurface(std::move(keyFrameInfo.radiusEnd), keyFrameInfo.pathEnd, axis);
	ParametricGeometry<StemSurface> endGeometry(endSurface, constraints);
	endGeometry.useNormals();
	endGeometry.useSTs();
	endGeometry.generateGeometry();

	// Create the morph targets as seperate geometries first
	StemSurface startSurface(std::move(keyFrameInfo.radiusEnd), keyFrameInfo.pathEnd, axis);
	ParametricGeometry<StemSurface> startGeometry(startSurface, constraints);
	startGeometry.useNormals();
	startGeometry.generateGeometry();

	StemSurface immatureStartSurface(std::move(keyFrameInfo.radiusStart), keyFrameInfo.pathStart, axis);
	ParametricGeometry<StemSurface> immatureStartGeometry(immatureStartSurface, constraints);
	immatureStartGeometry.useNormals();
	immatureStartGeometry.generateGeometry();

	StemSurface immatureEndSurface(std::move(keyFrameInfo.radiusStart), keyFrameInfo.pathStart, axis);
	ParametricGeometry<StemSurface> immatureEndGeometry(immatureEndSurface, constraints);
	immatureEndGeometry.useNormals();
	immatureEndGeometry.generateGeometry();

	// Move the morph targets into the main geometry object
	endGeometry.addMorphTarget("a", std::move(startGeometry.bufferAttributes.getBufferAttributeVec3("aVertexPosition").bufferData));
	endGeometry.addMorphTarget("b", std::move(immatureStartGeometry.bufferAttributes.getBufferAttributeVec3("aVertexPosition").bufferData));
	endGeometry.addMorphTarget("c", std::move(immatureEndGeometry.bufferAttributes.getBufferAttributeVec3("aVertexPosition").bufferData));

	return endGeometry;
}
}