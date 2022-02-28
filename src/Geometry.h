#pragma once

#include <vector>
#include <string>
#include <memory>

#include "BufferAttributes.h"

class Geometry {
private:
	bool useST, useNormal;

	std::vector<float> vertexBuffer;

public:
	Geometry();

	void addMorphTarget(const std::string &targetName, std::vector<glm::vec3> morphVertices);

	bool usesNormals();
	bool usesSTs();

	void useNormals();
	void useSTs();

	BufferAttributes bufferAttributes;
	std::vector<int> indexBuffer;

	virtual Geometry* clone() const;
	virtual ~Geometry();
};

Geometry mergeGeometry(std::vector<std::reference_wrapper<Geometry>>& geometries);