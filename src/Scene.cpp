#include "Scene.h"

void Scene::addEntity(EntityId entity) {
	entities.push_back(entity);
}

void Scene::removeEntity(EntityId entity) {
	for (int i = 0; i < entities.size(); i++) {

		if(entities[i] == entity) {
			entities.erase(entities.begin() + i);
			break;
		}
	}
}

Scene::Scene(const Camera& worldCamera) : camera(worldCamera) {};