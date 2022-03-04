#pragma once

#include <map>
#include <memory>
#include "Entity.h"

class EntityManager {
private:
	std::map<unsigned int, std::unique_ptr<Entity>> entities;
	unsigned int idCount;
public:
	EntityManager();
	Entity* getEntityById(unsigned int id);
	bool entityExists(unsigned int id) const;
	unsigned int addEntity(std::unique_ptr<Entity> newEntity);
	void removeEntity(unsigned int id);
};