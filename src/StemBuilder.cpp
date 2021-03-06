#include "StemBuilder.h"
#include <memory>
#include <cmath>
#include <iostream>

namespace StemBuilder {

StemRadius::StemRadius(float rStart, float rEnd, int branchLength, int shift) 
: radiusStart(rStart), radiusEnd(rEnd), noStems(branchLength), _shift(shift) {

	if (radiusEnd == radiusStart) {
		decayRate = 0.0f; // Radius is constant
	}
	else {
		decayRate = (-1.0f / noStems) * log((0.9f * radiusEnd) / (radiusStart - radiusEnd));
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
	return baseRadius(0.0f) * (float)(1.0f - (u * u));
}

StemTipRadiusStart* StemTipRadiusStart::clone() const {
	return new StemTipRadiusStart(*this);
}

StemTipRadiusEnd::StemTipRadiusEnd(float rStart, float rEnd, int branchLength, int shift) 
: StemRadius(rStart, rEnd, branchLength, shift) {}

float StemTipRadiusEnd::operator()(float u) const {
	return baseRadius(1.0f) * (float)(1.0f - (u * u)); 
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

	glm::vec3 crossSectionPoint = crossSection(u, v);

	return bezierPoint + crossSectionPoint;
}

glm::vec3 StemSurface::crossSection(float u, float v) const {
	float radius = _radiusFunc->operator()(u);
	glm::vec3 position = (_axis.left * radius * std::cos(v)) + (_axis.up * radius * std::sin(v));

	return position * (0.9f + (0.1f * std::pow(std::cos(3.0f * v), 2.0f)));
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

StemSurfaceMorphInfo::StemSurfaceMorphInfo(
	const StemSurfaceMorphInfo& other) : 
	pathStart(other.pathStart), 
	pathEnd(other.pathEnd) {

	radiusStart = std::unique_ptr<StemRadius>(other.radiusStart->clone());
	radiusEnd = std::unique_ptr<StemRadius>(other.radiusEnd->clone());
}

ParametricGeometry<StemSurface> generateStemBody(
	const LSystemParams &lParams, 
	const GeometryConstraints &constraints) {

	glm::vec3 normDir = glm::normalize(lParams.axis.forward);
	glm::vec3 p0 = lParams.position;
	glm::vec3 p1 = lParams.position + 0.03f * normDir;

	BezierLinear stemBodyPath(p0, p1);
	BezierLinear immatureStemBodyPath(p0, p0);

	// Define the parametric functions for radius over length of body
	StemBodyRadius startBodyRadiusFunc(0.001f, 0.001f, lParams.branchLength, 0);

	StemBodyRadius endBodyRadiusFunc(
		lParams.radiusStart,
		lParams.radiusEnd,
		lParams.branchLength,
		lParams.count
	);

	StemSurfaceMorphInfo keyFrameInfo(
		std::make_unique<StemBodyRadius>(startBodyRadiusFunc), 
		std::make_unique<StemBodyRadius>(endBodyRadiusFunc),
		immatureStemBodyPath,
		stemBodyPath
	);

	return StemBuilder::generateStemGeometry(std::move(keyFrameInfo), lParams.axis, constraints);
}

ParametricGeometry<StemSurface> generateStemBody(
	const LSystemParams & lParams, 
	const GeometryConstraints& constraints,
	Geometry& prev) {

	ParametricGeometry<StemSurface> geometry = StemBuilder::generateStemBody(lParams, constraints);
	connectStemBodyToPrev(geometry, prev, constraints);
	return geometry;
}

// Manually connect the top-row vertices of the previous stem body with the bottom-row vertices of the new stem
void connectStemBodyToPrev(
	Geometry& current,
	Geometry& prev,
	const GeometryConstraints& constraints) {

	int uSteps = constraints.uSteps;
	int offset = uSteps - 1;

	BufferAttributes& body = current.vertexBuffer;
	BufferAttributes& bodyPrev = prev.vertexBuffer;

	for (int i = 0; i < constraints.vSteps; i++) {

		int prevIndex = (uSteps * i) + offset;

		body.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[uSteps*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[prevIndex];
		body.getBufferAttribute<glm::vec3>("aNormal").bufferData[uSteps*i] = bodyPrev.getBufferAttribute<glm::vec3>("aNormal").bufferData[prevIndex];
		body.getBufferAttribute<glm::vec3>("aMatureStart").bufferData[uSteps*i] = bodyPrev.getBufferAttribute<glm::vec3>("aMatureStart").bufferData[prevIndex];
		body.getBufferAttribute<glm::vec3>("aImmatureStart").bufferData[uSteps*i] = bodyPrev.getBufferAttribute<glm::vec3>("aMatureStart").bufferData[prevIndex];
		body.getBufferAttribute<glm::vec3>("aImmatureEnd").bufferData[uSteps*i] = bodyPrev.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData[prevIndex];
	}
}

ParametricGeometry<StemSurface> generateStemTip(
	const LSystemParams &lParams, 
	const GeometryConstraints &constraints) {

	// Define the stem tip path direction in 3D space
	glm::vec3 normDir = glm::normalize(lParams.axis.forward);
	glm::vec3 p0 = lParams.position;
	glm::vec3 p1 = lParams.position + 0.03f * normDir;
	glm::vec3 p2 = lParams.position + 0.042f * normDir;
	glm::vec3 p3 = lParams.position + 0.012f * normDir;

	BezierLinear stemTipPath(p1, p2);
	BezierLinear immatureStemTipPath(p0, p3);

	// Define the parametric functions for radius over length of tip
	StemTipRadiusStart startTipRadiusFunc(0.001f, 0.001f, lParams.branchLength, 0);

	StemTipRadiusEnd endTipRadiusFunc(
		lParams.radiusStart,
		lParams.radiusEnd,
		lParams.branchLength,
		lParams.count
	);

	StemSurfaceMorphInfo keyFrameInfo(
		std::make_unique<StemTipRadiusStart>(startTipRadiusFunc), 
		std::make_unique<StemTipRadiusEnd>(endTipRadiusFunc),
		immatureStemTipPath,
		stemTipPath
	);

	return StemBuilder::generateStemGeometry(std::move(keyFrameInfo), lParams.axis, constraints);
}

ParametricGeometry<StemSurface> generateStemGeometry(
	const StemSurfaceMorphInfo &keyFrameInfo,
	const Axis& axis,
	const GeometryConstraints& constraints) {

	// Main geometry
	StemSurface endSurface(std::unique_ptr<StemRadius>(keyFrameInfo.radiusEnd->clone()), keyFrameInfo.pathEnd, axis);
	ParametricGeometry<StemSurface> endGeometry(endSurface, constraints);
	endGeometry.useNormals(false);
	endGeometry.useSTs();
	endGeometry.generateGeometry();

	
	// Create the morph targets as seperate geometries first
	StemSurface startSurface(std::unique_ptr<StemRadius>(keyFrameInfo.radiusStart->clone()), keyFrameInfo.pathEnd, axis);
	ParametricGeometry<StemSurface> startGeometry(startSurface, constraints);
	startGeometry.generateGeometry();

	StemSurface immatureStartSurface(std::unique_ptr<StemRadius>(keyFrameInfo.radiusStart->clone()), keyFrameInfo.pathStart, axis);
	ParametricGeometry<StemSurface> immatureStartGeometry(immatureStartSurface, constraints);
	immatureStartGeometry.generateGeometry();

	StemSurface immatureEndSurface(std::unique_ptr<StemRadius>(keyFrameInfo.radiusEnd->clone()), keyFrameInfo.pathStart, axis);
	ParametricGeometry<StemSurface> immatureEndGeometry(immatureEndSurface, constraints);
	immatureEndGeometry.generateGeometry();

	// Move the morph targets into the main geometry object
	endGeometry.addMorphTarget(
		"aMatureStart", 
		std::move(startGeometry.vertexBuffer.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData)
	);
	
	endGeometry.addMorphTarget(
		"aImmatureStart",
		std::move(immatureStartGeometry.vertexBuffer.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData)
	);
	endGeometry.addMorphTarget(
		"aImmatureEnd",
		std::move(immatureEndGeometry.vertexBuffer.getBufferAttribute<glm::vec3>("aVertexPosition").bufferData)
	);
	
	return endGeometry;
}
}