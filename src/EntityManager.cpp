#include "EntityManager.h"
#include <cassert>
#include <utility>

EntityManager::EntityManager(BufferManager& bufferManager, Scene& scene) : idCount(0), _bufferManager(bufferManager), _scene(scene) {};

Entity& EntityManager::getEntityById(EntityId id) {
	return *entities.at(id);
}

bool EntityManager::entityExists(EntityId id) const {
	return entities.find(id) != entities.end();
}

EntityId EntityManager::addEntity(std::unique_ptr<Entity> newEntity) {
	idCount++;
	entities.emplace(std::make_pair(idCount, std::move(newEntity)));
	return idCount;
}

void EntityManager::removeEntity(EntityId id) {
	Entity& entity = getEntityById(id);
	Geometry* geometry = entity.worldObject.getMesh()._geometry.get();
	int bufferId = entity.worldObject.getMesh()._geometry->bufferId;
	_bufferManager.getBufferById(bufferId).removeBufferGeometry(geometry);

	entities.erase(id);
}

void EntityManager::addEntityToScene(EntityId id) {
	_scene.addEntity(getEntityById(id).worldObject);
	activeEntities.push_back(id);
}

void EntityManager::removeEntityFromScene(EntityId id) {
	Entity& entity = getEntityById(id);
	Object3D* worldObject = &entity.worldObject;
}

// Returns all entities currently active
std::vector<EntityId>& EntityManager::getActiveEntities() {
	return activeEntities;
}