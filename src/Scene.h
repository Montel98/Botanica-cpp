#pragma once

#include <vector>
#include "Camera.h"

using EntityId = unsigned int;

class Scene {
private:
	std::vector<EntityId> entities;
public:
	Camera camera;
	Scene(const Camera& worldCamera);
	void addEntity(EntityId entity);
	void removeEntity(EntityId entity);
};