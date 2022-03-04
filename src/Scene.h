#pragma once

#include <vector>

using EntityId = unsigned int;

class Scene {
private:
	std::vector<EntityId> entities;
public:
	void addEntity(EntityId entity);
	void removeEntity(EntityId entity);
};