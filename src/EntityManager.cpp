#include "EntityManager.h"
#include <cassert>
#include <utility>

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
	entities.erase(id);
}