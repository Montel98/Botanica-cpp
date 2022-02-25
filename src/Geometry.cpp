#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Geometry.h"

class Buffer {
private:
	std::string BufferName;
	unsigned int bufferID;
};


Geometry::Geometry() : useST(false), useNormal(false) {
	bufferAttributes.addBufferAttributeVec3("aVertexPosition");
}

void Geometry::addMorphTarget(const std::string &targetName, std::vector<glm::vec3> morphVertices) {
	bufferAttributes.addBufferAttributeVec3(targetName);
	bufferAttributes.setBufferAttributeDataVec3(targetName, morphVertices);
}

bool Geometry::usesNormals() {
	return useNormal;
}

bool Geometry::usesSTs() {
	return useST;
}

void Geometry::useSTs() {
	useST = true;
	bufferAttributes.addBufferAttributeVec2("aTexCoord");
}

void Geometry::useNormals() {
	useNormal = true;
	bufferAttributes.addBufferAttributeVec3("aNormal");
}

Geometry* Geometry::clone() const {
	return new Geometry(*this);
}

Geometry::~Geometry() {}

Geometry Geometry::mergeGeometry(const std::vector<std::reference_wrapper<Geometry>>& geometries) {
	return Geometry();
}