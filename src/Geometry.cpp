#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Geometry.h"
#include <iostream>

class Buffer {
private:
	std::string BufferName;
	unsigned int bufferID;
};


Geometry::Geometry() : useST(false), useNormal(false) {
	bufferAttributes.addBufferAttribute<glm::vec3>("aVertexPosition");
}

void Geometry::addMorphTarget(const std::string &targetName, std::vector<glm::vec3> morphVertices) {
	bufferAttributes.addBufferAttribute<glm::vec3>(targetName);
	bufferAttributes.setBufferAttributeData<glm::vec3>(targetName, morphVertices);
}

bool Geometry::usesNormals() {
	return useNormal;
}

bool Geometry::usesSTs() {
	return useST;
}

void Geometry::useSTs() {
	useST = true;
	bufferAttributes.addBufferAttribute<glm::vec2>("aTexCoord");
}

void Geometry::useNormals() {
	useNormal = true;
	bufferAttributes.addBufferAttribute<glm::vec3>("aNormal");
}

Geometry* Geometry::clone() const {
	return new Geometry(*this);
}

Geometry::~Geometry() {}

// Merges any type of geometry provided they share the same buffer attributes
// A generic geometry container is returned, any geometry specific info is lost
Geometry mergeGeometry(std::vector<std::reference_wrapper<Geometry>>& geometries) {

	// Copy first geometry in list to merged geometry
	Geometry mergedGeometry;

	if (geometries.size() > 0) {
		Geometry& firstGeometry = geometries[0].get();
		mergedGeometry.bufferAttributes = firstGeometry.bufferAttributes;
		mergedGeometry.indexBuffer = firstGeometry.indexBuffer;
	}

	for(std::reference_wrapper<Geometry>& geometry : geometries) {

		BufferAttributes& otherBufferAttributes = geometry.get().bufferAttributes;
		mergedGeometry.bufferAttributes.mergeBufferAttributes(otherBufferAttributes);
	}

	return mergedGeometry;
}