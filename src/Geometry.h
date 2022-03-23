#pragma once

#include <vector>
#include <string>
#include <memory>

#include "BufferAttributes.h"

struct GeometryEvent {
	int vertexStart, vertexLength;
	int indexStart, indexLength;
};

class Geometry {
private:
	bool useST, useNormal;
public:
	Geometry();

	void addMorphTarget(const std::string &targetName, std::vector<glm::vec3> morphVertices);
	void addMorphTarget(
		const std::string &targetName, 
		std::vector<glm::vec3> morphVertices, 
		std::vector<glm::vec3> morphNormals
	);

	void addGeometry(Geometry& other);
	Geometry sliceGeometry(unsigned int start, unsigned int length);

	bool usesNormals();
	bool usesSTs();

	void useNormals();
	void useSTs();

	void updateGeometry();

	BufferAttributes bufferAttributes;
	BufferAttributes indexBuffer;

	std::string bufferName;
	int bufferId;

	std::vector<GeometryEvent> modificationEvents;
	void addGeometryEvent(int vertexStart, int vertexLength, int indexStart, int indexLength);

	virtual Geometry* clone() const;
	virtual ~Geometry();
};

Geometry mergeGeometry(std::vector<std::reference_wrapper<Geometry>>& geometries);