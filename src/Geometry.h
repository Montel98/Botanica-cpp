#pragma once

#include <vector>
#include <string>
#include <memory>

#include "BufferAttributes.h"

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

	bool usesNormals();
	bool usesSTs();

	void useNormals();
	void useSTs();

	void updateGeometry();

	BufferAttributes bufferAttributes;
	BufferAttributes indexBuffer;

	std::string bufferName;
	int bufferId;

	bool geometryModified;

	virtual Geometry* clone() const;
	virtual ~Geometry();
};

Geometry mergeGeometry(std::vector<std::reference_wrapper<Geometry>>& geometries);