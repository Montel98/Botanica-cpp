#pragma once

#include <vector>
#include "Camera.h"
#include "Object3D.h"

using EntityId = unsigned int;

class Scene {
public:
	Camera camera;
	//std::vector<EntityId> entities;
	std::vector<Object3D*> entities;
	
	Scene(const Camera& worldCamera);
	void addEntity(/*EntityId entity*/Object3D& object3D);
	void removeEntity(/*EntityId entity*/Object3D& object3D);
};