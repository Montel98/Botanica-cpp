#pragma once

#include <glm/glm.hpp>
#include "Geometry.h"
#include <memory>

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
	GeometryConstraints limits;
	STMapping mapping;
	float deltaU, deltaV;

	std::vector<glm::vec2> generateSTs() const;
	std::vector<glm::vec3> generateNormals() const;
	std::vector<glm::vec3> generateVertices() const;
	std::vector<int> generateIndices() const;

public:
	ParametricGeometry(const F &surfaceFunc, const GeometryConstraints &constraints);
	virtual ParametricGeometry<F>* clone() const override;
	virtual ~ParametricGeometry<F>();

	// Concatenates all vertex attributes and populates the vertex + index buffer
	void generateGeometry();

	void useSTMapping(const STMapping &mappingST);
};

template <typename F>
ParametricGeometry<F>::ParametricGeometry(const F &surfaceFunc, const GeometryConstraints &constraints) :
surface(surfaceFunc), limits(constraints), mapping({0.0, 1.0, 0.0, 1.0}) {

	deltaU = (limits.uMax - limits.uMin) / (limits.uSteps - 1.0);
	deltaV = (limits.vMax - limits.vMin) / (limits.vSteps - 1.0);
}

template <typename F>
std::vector<glm::vec2> ParametricGeometry<F>::generateSTs() const {

	std::vector<glm::vec2> newSTs;
	
	for (auto vStep = 0; vStep < limits.vSteps; vStep++) {

		for (auto uStep = 0; uStep < limits.uSteps; uStep++) {

			float u = limits.uMin + (uStep * deltaU);
			float v = limits.vMin + (vStep * deltaV);

			float s = mapping.sMin + ((mapping.sMax - mapping.sMin) * (u / (limits.uSteps - 1)));
			float t = mapping.tMin + ((mapping.tMax - mapping.tMin) * (v / (limits.vSteps - 1)));

			newSTs.push_back(glm::vec2(s, t));
		}
	}

	return newSTs;
}

template <typename F>
std::vector<glm::vec3> ParametricGeometry<F>::generateNormals() const {

	std::vector<glm::vec3> newNormals;
	float epsilon = 0.0001;

	for (auto vStep = 0; vStep < limits.vSteps; vStep++) {

		for (auto uStep = 0; uStep < limits.uSteps; uStep++) {

			float u = limits.uMin + (uStep * deltaU);
			float v = limits.vMin + (vStep * deltaV);

			glm::vec3 vertex = surface(u, v);

			glm::vec3 du = surface(u + epsilon, v) - vertex;
			glm::vec3 dv = surface(u, v + epsilon) - vertex;

			glm::vec3 normal = cross(dv, du);

			if (glm::vec3(0,0,0) == normal) {
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

	for (auto vStep = 0; vStep < limits.vSteps; vStep++) {

		for (auto uStep = 0; uStep < limits.uSteps; uStep++) {

			float u = limits.uMin + (uStep * deltaU);
			float v = limits.vMin + (vStep * deltaV);

			glm::vec3 vertex = surface(u, v);

			newVertices.push_back(vertex);
		}
	}

	return newVertices;
}

template <typename F>
std::vector<int> ParametricGeometry<F>::generateIndices() const {

	std::vector<int> newIndices;

	for (auto vStep = 0; vStep < limits.vSteps; vStep++) {

		for (auto uStep = 0; uStep < limits.uSteps; uStep++) {

			for (auto i = 0; i < 6; i++) {

				float indexA = (vStep * limits.uSteps) + uStep;
				float indexB = (vStep * limits.uSteps) + (uStep + 1);
				float indexC = ((vStep + 1) * limits.uSteps) + (uStep + 1);
				float indexD = ((vStep + 1) * limits.uSteps) + uStep;

				newIndices.push_back(indexA);
				newIndices.push_back(indexB);
				newIndices.push_back(indexC);

				newIndices.push_back(indexC);
				newIndices.push_back(indexD);
				newIndices.push_back(indexA);
			}
		}
	}

	return newIndices;
}

template <typename F>
void ParametricGeometry<F>::generateGeometry() {

	//bufferAttributes.setBufferAttributeDataVec3("aVertexPosition", std::move(generateVertices()));
	bufferAttributes.setBufferAttributeData<glm::vec3>("aVertexPosition", generateVertices());
	indexBuffer.bufferData = generateIndices();

	if (usesNormals()) {
		//std::vector<glm::vec3> normals = generateNormals();
		bufferAttributes.setBufferAttributeData<glm::vec3>("aNormal", generateNormals());
	}

	if (usesSTs()) {
		//std::vector<glm::vec2> STs = generateSTs();
		bufferAttributes.setBufferAttributeData<glm::vec2>("aTexCoord", generateSTs());
	}
}

template<typename F>
void ParametricGeometry<F>::useSTMapping(const STMapping &mappingST) {
	mapping = mappingST;
}

template<typename F>
ParametricGeometry<F>* ParametricGeometry<F>::clone() const {
	return new ParametricGeometry(*this);
}

template<typename F>
ParametricGeometry<F>::~ParametricGeometry<F>() {};