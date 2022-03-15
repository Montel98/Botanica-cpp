#pragma once

#include <vector>
#include "Camera.h"

using EntityId = unsigned int;

class Scene {
public:
	Camera camera;
	std::vector<EntityId> entities;
	
	Scene(const Camera& worldCamera);
	void addEntity(EntityId entity);
	void removeEntity(EntityId entity);
};