#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Geometry.h"
#include <iostream>

Geometry::Geometry() : useST(false), useNormal(false), bufferName(""), bufferId(-1) {
	bufferAttributes.addBufferAttribute<glm::vec3>("aVertexPosition");
	indexBuffer.addBufferAttribute<glm::ivec1>("aIndex");
}

void Geometry::addMorphTarget(const std::string &targetName, std::vector<glm::vec3> morphVertices) {
	bufferAttributes.addBufferAttribute<glm::vec3>(targetName);
	bufferAttributes.setBufferAttributeData<glm::vec3>(targetName, morphVertices);
}

void Geometry::addMorphTarget(
		const std::string &targetName, 
		std::vector<glm::vec3> morphVertices, 
		std::vector<glm::vec3> morphNormals) {

	std::string vertexName = "aVertex" + targetName;
	std::string normalName = "aNormal" + targetName;

	bufferAttributes.addBufferAttribute<glm::vec3>(vertexName);
	bufferAttributes.setBufferAttributeData<glm::vec3>(vertexName, morphVertices);

	bufferAttributes.addBufferAttribute<glm::vec3>(normalName);
	bufferAttributes.setBufferAttributeData<glm::vec3>(normalName, morphNormals);
}

bool Geometry::usesNormals() const {
	return useNormal;
}

bool Geometry::normalsInverted() const {
	return invertedNormals;
}

bool Geometry::usesSTs() const {
	return useST;
}

void Geometry::useSTs() {
	useST = true;
	bufferAttributes.addBufferAttribute<glm::vec2>("aTexCoord");
}

void Geometry::useNormals(bool inverted) {
	useNormal = true;
	invertedNormals = inverted;
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

	for (int i = 1; i < geometries.size(); i++) {
		mergedGeometry.addGeometry(geometries[i].get());
	}

	return mergedGeometry;
}

void Geometry::addGeometry(Geometry& other) {

		int vertexStart = bufferAttributes.getNoElements();
		int indexStart = indexBuffer.getNoElements();

		BufferAttributes& otherBufferAttributes = other.bufferAttributes;

		// Make copy of index buffer of geometry to be merged and shift indices
		BufferAttributes otherIndexBuffer = other.indexBuffer;
		
		for (glm::ivec1& index : otherIndexBuffer.getBufferAttribute<glm::ivec1>("aIndex").bufferData) {
			int noElements = this->bufferAttributes.getLength() / this->bufferAttributes.getStride();
			index += noElements;
		}

		this->indexBuffer.appendBufferAttributeData<glm::ivec1>("aIndex", otherIndexBuffer.getBufferAttribute<glm::ivec1>("aIndex").bufferData);

		// Merge all other attributes
		this->bufferAttributes.mergeBufferAttributes(otherBufferAttributes);

		int vertexLength = bufferAttributes.getNoElements() - vertexStart;
		int indexLength = indexBuffer.getNoElements() - indexStart;

		addGeometryEvent(vertexStart, vertexLength, indexStart, indexLength);
}

Geometry Geometry::sliceGeometry(unsigned int start, unsigned int length) {
	Geometry slicedGeometry(*this);
	slicedGeometry.bufferId = -1;
	slicedGeometry.bufferName = "";
	slicedGeometry.bufferAttributes.sliceBufferAttributes(start, length);
	slicedGeometry.indexBuffer.sliceBufferAttributes(6 * start, 6 * (length - 1));

	return slicedGeometry;
}

void Geometry::addGeometryEvent(int vertexStart, int vertexLength, int indexStart, int indexLength) {
	modificationEvents.push_back(GeometryEvent{vertexStart, vertexLength, indexStart, indexLength});
}