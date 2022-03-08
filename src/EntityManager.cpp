#include "EntityManager.h"
#include <cassert>
#include <utility>

EntityManager::EntityManager(BufferManager& bufferManager) : idCount(0), _bufferManager(bufferManager) {};

Entity* EntityManager::getEntityById(unsigned int id) {
	return entities[id].get();
}

bool EntityManager::entityExists(unsigned int id) const {
	return entities.find(id) != entities.end();
}

unsigned int EntityManager::addEntity(std::unique_ptr<Entity> newEntity) {
	idCount++;
	entities.emplace(std::make_pair(idCount, std::move(newEntity)));
	return idCount;
}

void EntityManager::removeEntity(unsigned int id) {
	Entity* entity = getEntityById(id);
	Geometry* geometry = entity->worldObject.getMesh()._geometry.get();
	int bufferId = entity->worldObject.getMesh()._geometry->bufferId;
	_bufferManager.getBufferById(bufferId).removeEntityBuffer(geometry);

	entities.erase(id);
}