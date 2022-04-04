#pragma once

#include <glm/glm.hpp>
#include "Geometry.h"
#include <memory>
#include <iostream>

// Defines bounds and granularity for 2 variable parametric function
struct GeometryConstraints {
	float uMin, uMax, vMin, vMax;
	int uSteps, vSteps;
};

// Defines bounds for texture coordinates
struct STMapping {
	float sMin, sMax, tMin, tMax;
};

template <typename F>
class ParametricGeometry : public Geometry {
private:
	F surface;
	STMapping mapping;
	float deltaU, deltaV;

	std::vector<glm::vec2> generateSTs() const;
	std::vector<glm::vec3> generateNormals() const;
	std::vector<glm::vec3> generateVertices() const;
	std::vector<glm::ivec1> generateIndices() const;

public:
	ParametricGeometry(const F& surfaceFunc, const GeometryConstraints& constraints);
	virtual ParametricGeometry<F>* clone() const override;
	virtual ~ParametricGeometry<F>();

	// Concatenates all vertex attributes and populates the vertex + index buffer
	void generateGeometry();
	void setSTMap(const STMapping &mappingST);

	GeometryConstraints limits;
};

template <typename F>
ParametricGeometry<F>::ParametricGeometry(const F& surfaceFunc, const GeometryConstraints& constraints) :
surface(surfaceFunc), limits(constraints), mapping({0.0, 1.0, 0.0, 1.0}) {

	deltaU = (limits.uMax - limits.uMin) / (limits.uSteps - 1.0);
	deltaV = (limits.vMax - limits.vMin) / (limits.vSteps - 1.0);
}

template <typename F>
std::vector<glm::vec2> ParametricGeometry<F>::generateSTs() const {

	std::vector<glm::vec2> newSTs;

	std::cout << "sMin: " << mapping.sMin << "sMax: " << mapping.sMax << "tMin: " << mapping.tMin << "tMax: " << mapping.tMax << "\n";
	
	for (int vStep = 0; vStep < limits.vSteps; vStep++) {

		for (int uStep = 0; uStep < limits.uSteps; uStep++) {

			//float u = limits.uMin + (uStep * deltaU);
			//float v = limits.vMin + (vStep * deltaV);

			float s = mapping.sMin + ((mapping.sMax - mapping.sMin) * (uStep / (limits.uSteps - 1)));
			float t = mapping.tMin + ((mapping.tMax - mapping.tMin) * (vStep / (limits.vSteps - 1)));

			std::cout << s << ",,," << t << "\n";

			newSTs.push_back(glm::vec2(s, t));
		}
	}

	return newSTs;
}

template <typename F>
std::vector<glm::vec3> ParametricGeometry<F>::generateNormals() const {

	std::vector<glm::vec3> newNormals;
	float epsilon = 0.0001;

	for (int vStep = 0; vStep < limits.vSteps; vStep++) {

		for (int uStep = 0; uStep < limits.uSteps; uStep++) {

			float u = limits.uMin + (uStep * deltaU);
			float v = limits.vMin + (vStep * deltaV);

			glm::vec3 vertex = surface(u, v);

			glm::vec3 du = surface(u + epsilon, v) - vertex;
			glm::vec3 dv = surface(u, v + epsilon) - vertex;

			glm::vec3 normal = normalsInverted() ? cross(du, dv) : cross(dv, du);

			if (glm::vec3(0,0,0) != normal) {
				normal = glm::normalize(normal);
			}

			newNormals.push_back(normal);
		}
	}

	return newNormals;
}

template <typename F>
std::vector<glm::vec3> ParametricGeometry<F>::generateVertices() const {

	std::vector<glm::vec3> newVertices;

	for (int vStep = 0; vStep < limits.vSteps; vStep++) {

		for (int uStep = 0; uStep < limits.uSteps; uStep++) {

			float u = limits.uMin + (uStep * deltaU);
			float v = limits.vMin + (vStep * deltaV);

			glm::vec3 vertex = surface(u, v);

			//std::cout << u << "," << v << " -> " << vertex.x << "," << vertex.y << "," << vertex.z << "\n";

			newVertices.push_back(vertex);
		}
	}

	//std::cout << "\n";

	return newVertices;
}

template <typename F>
std::vector<glm::ivec1> ParametricGeometry<F>::generateIndices() const {

	std::vector<glm::ivec1> newIndices;

	for (int vStep = 0; vStep < limits.vSteps - 1; vStep++) {

		for (int uStep = 0; uStep < limits.uSteps - 1; uStep++) {

			int indexA = (vStep * limits.uSteps) + uStep;
			int indexB = (vStep * limits.uSteps) + (uStep + 1);
			int indexC = ((vStep + 1) * limits.uSteps) + (uStep + 1);
			int indexD = ((vStep + 1) * limits.uSteps) + uStep;

			newIndices.push_back(glm::ivec1(indexA));
			newIndices.push_back(glm::ivec1(indexB));
			newIndices.push_back(glm::ivec1(indexC));

			newIndices.push_back(glm::ivec1(indexC));
			newIndices.push_back(glm::ivec1(indexD));
			newIndices.push_back(glm::ivec1(indexA));
		}
	}

	return newIndices;
}

template <typename F>
void ParametricGeometry<F>::generateGeometry() {

	vertexBuffer.setBufferAttributeData<glm::vec3>("aVertexPosition", generateVertices());
	indexBuffer.setBufferAttributeData<glm::ivec1>("aIndex", generateIndices());

	if (usesNormals()) {
		//std::vector<glm::vec3> normals = generateNormals();
		vertexBuffer.setBufferAttributeData<glm::vec3>("aNormal", generateNormals());
	}

	if (usesSTs()) {
		//std::vector<glm::vec2> STs = generateSTs();
		vertexBuffer.setBufferAttributeData<glm::vec2>("aTexCoord", generateSTs());
	}
}

template<typename F>
void ParametricGeometry<F>::setSTMap(const STMapping &mappingST) {
	mapping = mappingST;
}

template<typename F>
ParametricGeometry<F>* ParametricGeometry<F>::clone() const {
	return new ParametricGeometry(*this);
}

template<typename F>
ParametricGeometry<F>::~ParametricGeometry<F>() {};