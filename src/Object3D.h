#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

#pragma once

class Object3D {
private:
	Mesh _mesh;
public:
	Object3D* parent;
	std::vector<Object3D*> children;
	Object3D(Mesh mesh);

	bool isHidden;

	void addChild(Object3D* child);
	void removeChild(Object3D* child);

	void hide();

	void show();

	Mesh &getMesh();

	glm::mat4 localMatrix;
	glm::mat4 worldMatrix;
};