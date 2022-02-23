#pragma once

#include <vector>
#include <string>

#include "BufferAttributes.h"

class Geometry {
private:
	bool useST, useNormal;

	std::vector<float> vertexBuffer;
	std::vector<int> indexBuffer;

public:
	Geometry();

	void addMorphTarget(const std::string &targetName, const std::vector<glm::vec3>&morphVertices);

	bool usesNormals();
	bool usesSTs();

	void useNormals();
	void useSTs();

	BufferAttributes bufferAttributes;
};