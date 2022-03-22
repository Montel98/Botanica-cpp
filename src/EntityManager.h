#pragma once

#include <map>
#include <memory>
#include "Entity.h"
#include "Buffer.h"
#include "Scene.h"

using EntityId = unsigned int;

class EntityManager {
private:
	std::map<EntityId, std::unique_ptr<Entity>> entities;
	std::vector<EntityId> activeEntities;
	EntityId idCount;
	BufferManager& _bufferManager;
	Scene& _scene;
public:
	EntityManager(BufferManager& bufferManager, Scene& scene);
	Entity* getEntityById(EntityId id);
	bool entityExists(EntityId id) const;
	EntityId addEntity(std::unique_ptr<Entity> newEntity);
	void addEntityToScene(EntityId id);
	void removeEntityFromScene(EntityId id);
	void removeEntity(EntityId id);
	std::vector<EntityId>& getActiveEntities();
};